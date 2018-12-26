#include <stdafx.h>
#include <ogele.h>
namespace ogele {
	std::map<std::string, RenderPipeline::PassMode> RenderPipeline::StrToMode = {
		{"scene",RenderPipeline::PassMode::Scene},
		{"screen",RenderPipeline::PassMode::Screen},
		{"compute",RenderPipeline::PassMode::Compute}
	};

	RenderPipeline::RenderPipeline() {
		m_material = std::make_unique<Material>();
	}

	void DrawRecursive(const Camera* cam, Transform* node, const std::vector<std::string>& tags) {
		if (node->GetActor() != nullptr)
			for (const auto& c : node->GetActor()->GetComponents()) {
				Renderable* r = dynamic_cast<Renderable*>(c.get());
				if (r != nullptr)
					r->Render(cam, tags);
			}
		for (const auto& t : node->GetChilds())
			DrawRecursive(cam, t.get(), tags);
	}

	void RenderPipeline::Init() const {
		for (const auto& pass : m_inits) {
			Disable(Feature::CullFace);
			Disable(Feature::DepthTest);
			if (pass.Mode == PassMode::Compute) {
				pass.SSShader->Bind();
				m_material->Apply(pass.SSShader.get());
				Application::GetMaterial()->Apply(pass.SSShader.get());
				pass.SSShader->bDispatchCompute(pass.GroupNum);
				pass.SSShader->Unbind();
			}
			else if (pass.Mode == PassMode::Screen) {
				pass.Target->Bind();
				Viewport({ 0,0 }, pass.Target->GetSize());
				if (pass.Clear != (BufferBit)0) {
					ClearColor({ 0.4f, 0.6f, 0.8f, 1.0f });
					Clear(pass.Clear);
				}
				pass.SSShader->Bind();
				m_material->Apply(pass.SSShader.get());
				Application::GetMaterial()->Apply(pass.SSShader.get());
				Application::DrawScreenQuad();
				pass.SSShader->Unbind();
				break;
				pass.Target->Unbind();
			}
			for (TextureBase* tex : pass.Mipmap) {
				tex->Bind();
				tex->bGenerateMipMap();
				tex->Unbind();
			}
		}
	}

	void RenderPipeline::Render(Camera* cam) const {
		cam->UpdateMaterial();
		for (const auto& pass : m_passes) {
			switch (pass.Mode) {
			case PassMode::Scene:
				Enable(Feature::CullFace);
				Enable(Feature::DepthTest);
				break;
			case PassMode::Screen:
				Disable(Feature::CullFace);
				Disable(Feature::DepthTest);
				break;
			}
			if (pass.Mode == PassMode::Compute) {
				pass.SSShader->Bind();
				m_material->Apply(pass.SSShader.get());
				cam->GetMaterial()->Apply(pass.SSShader.get());
				Application::GetMaterial()->Apply(pass.SSShader.get());
				pass.SSShader->bDispatchCompute(pass.GroupNum);
				pass.SSShader->Unbind();
			}
			else {
				pass.Target->Bind();
				Viewport({ 0,0 }, pass.Target->GetSize());
				if (pass.Clear != (BufferBit)0) {
					ClearColor({ 0.4f, 0.6f, 0.8f, 1.0f });
					Clear(pass.Clear);
				}
				switch (pass.Mode) {
				case PassMode::Scene:
					for (const auto& root : Application::GetRoots())
						DrawRecursive(cam, root.get(), pass.Tags);
					break;
				case PassMode::Screen:
					pass.SSShader->Bind();
					m_material->Apply(pass.SSShader.get());
					cam->GetMaterial()->Apply(pass.SSShader.get());
					Application::GetMaterial()->Apply(pass.SSShader.get());
					Application::DrawScreenQuad();
					pass.SSShader->Unbind();
					break;
				}
				pass.Target->Unbind();
			}
			for (TextureBase* tex : pass.Mipmap) {
				tex->Bind();
				tex->bGenerateMipMap();
				tex->Unbind();
			}
		}
		Application::DrawTex((*m_renderTargets.at("Final").RenderTarget.get())[0]);
	}

	void RenderPipeline::Resize(const glm::ivec2& newSize) {
		for (auto& t : m_renderTargets)
			t.second.RenderTarget->Resize((ivec2)(t.second.Size * (vec2)newSize));
	}
}