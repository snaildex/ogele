#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {
	RenderPipeline::RenderPipeline(const fs::path& sourcePath) {
		m_cam = nullptr;
		m_world = nullptr;
		m_finalTarget = nullptr;
		m_finalTargetTextureIndex = -1;
		m_material = make_unique<Material>();
		m_lua = make_unique<Lua::State>(sourcePath);
		m_lua->SetName("Render pipeline script");
		m_lua->SetMethod("createTexture", this, &RenderPipeline::CreateTexture);
		m_lua->SetMethod("linkTexture", this, &RenderPipeline::LinkTexture);
		m_lua->SetMethod("createRenderTarget", this, &RenderPipeline::CreateRenderTarget);
		m_lua->SetMethod("findShader", this, &RenderPipeline::FindShader);
		m_lua->SetMethod("enable", this, &RenderPipeline::Enable);
		m_lua->SetMethod("disable", this, &RenderPipeline::Disable);
		m_lua->SetMethod("blendFunc", this, &RenderPipeline::BlendFunc);
		m_lua->SetMethod("dispatchCompute", this, &RenderPipeline::DispatchCompute);
		m_lua->SetMethod("drawScreen", this, &RenderPipeline::DrawScreen);
		m_lua->SetMethod("drawScene", this, &RenderPipeline::DrawScene);
		m_lua->SetMethod("setFinalRenderTarget", this, &RenderPipeline::SetFinalRenderTarget);
		m_lua->SetMethod("clearRenderTarget", this, &RenderPipeline::ClearRenderTarget);
		m_lua->SetMethod("resizeRenderTarget", this, &RenderPipeline::ResizeRenderTarget);
		for (auto t : StrToTexFormat) m_lua->SetGlobalVar(t.first, (int)t.second);
		for (auto t : StrToTexType) m_lua->SetGlobalVar(t.first, (int)t.second);
		for (auto t : StrToTexFilterMode) m_lua->SetGlobalVar(t.first, (int)t.second);
		for (auto t : StrToTexWrapMode) m_lua->SetGlobalVar(t.first, (int)t.second);
		m_lua->CreateTable(0, 3);
		m_lua->TableField("blend", (int)Feature::Blend);
		m_lua->TableField("depthTest", (int)Feature::DepthTest);
		m_lua->TableField("cullFace", (int)Feature::CullFace);
		m_lua->SetGlobal("feature");
		m_lua->CreateTable(0, 2);
		m_lua->TableField("width", Application::GetInstance()->GetResolution().x);
		m_lua->TableField("height", Application::GetInstance()->GetResolution().y);
		m_lua->SetGlobal("screen");
		m_lua->CreateTable(0, 4);
		m_lua->TableField("accumulation", (int)BufferBit::Accumulation);
		m_lua->TableField("color", (int)BufferBit::Color);
		m_lua->TableField("depth", (int)BufferBit::Depth);
		m_lua->TableField("stencil", (int)BufferBit::Stencil);
		m_lua->SetGlobal("renderTargetBuffer");
		m_lua->CreateTable(0, 19);
		m_lua->TableField("constantAlpha", (int)BlendFactor::ConstantAlpha);
		m_lua->TableField("constantColor", (int)BlendFactor::ConstantColor);
		m_lua->TableField("dstAlpha", (int)BlendFactor::DstAlpha);
		m_lua->TableField("dstColor", (int)BlendFactor::DstColor);
		m_lua->TableField("oneMinusConstantAlpha", (int)BlendFactor::OneMinusConstantAlpha);
		m_lua->TableField("oneMinusConstantColor", (int)BlendFactor::OneMinusConstantColor);
		m_lua->TableField("oneMinusDstAlpha", (int)BlendFactor::OneMinusDstAlpha);
		m_lua->TableField("oneMinusDstColor", (int)BlendFactor::OneMinusDstColor);
		m_lua->TableField("oneMinusSrc1Alpha", (int)BlendFactor::OneMinusSrc1Alpha);
		m_lua->TableField("oneMinusSrc1Color", (int)BlendFactor::OneMinusSrc1Color);
		m_lua->TableField("oneMinusSrcAlpha", (int)BlendFactor::OneMinusSrcAlpha);
		m_lua->TableField("oneMinusSrcColor", (int)BlendFactor::OneMinusSrcColor);
		m_lua->TableField("src1Alpha", (int)BlendFactor::Src1Alpha);
		m_lua->TableField("src1Color", (int)BlendFactor::Src1Color);
		m_lua->TableField("srcAlpha", (int)BlendFactor::SrcAlpha);
		m_lua->TableField("srcAlphaSaturate", (int)BlendFactor::SrcAlphaSaturate);
		m_lua->TableField("srcColor", (int)BlendFactor::SrcColor);
		m_lua->TableField("zero", (int)BlendFactor::Zero);
		m_lua->TableField("one", (int)BlendFactor::One);
		m_lua->SetGlobal("blendFactor");
		m_lua->Load();
	}

	RenderPipeline::~RenderPipeline() {
	}

	void RenderPipeline::Render()
	{
		if (m_cam != nullptr) m_cam->UpdateMaterial();
		m_lua->Call("render");
		ogele::Disable(Feature::Blend);
		ogele::BlendFunc(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);
		if (m_finalTarget != nullptr && m_finalTargetTextureIndex >= 0)
			Application::DrawTex((*m_finalTarget)[m_finalTargetTextureIndex]);
	}

	void RenderPipeline::Resize(const ivec2 & size)
	{
		m_lua->GetGlobal("screen");
		m_lua->TableField("width", size.x);
		m_lua->TableField("height", size.y);
		m_lua->SetGlobal("screen");
		m_lua->Call("resize");
	}

	int RenderPipeline::CreateTexture(Lua::State * state)
	{
		auto cdir = fs::current_path();
		fs::current_path(m_lua->GetSourcePath().parent_path());
		TextureType type = (TextureType)state->Peek<int>(1);
		string name = state->Peek<string>(2);
		TextureFormat format = TextureFormat::RGBA8;
		TextureFilterMode minFilter = TextureFilterMode::LinearMipMapNearest;
		TextureFilterMode magFilter = TextureFilterMode::Linear;
		TextureWrapMode wrap = TextureWrapMode::ClampToEdge;
		bool mipmaps = true;
		ivec3 size = { -1,-1,-1 };
		state->PeekArray(3, [&](int index) { size[index - 1] = state->Peek<int>(-1); });
		state->PeekTable(4, [&](const string& field) {
			if (field == "format") format = (TextureFormat)state->Peek<int>(-1);
			if (field == "minFilter") minFilter = (TextureFilterMode)state->Peek<int>(-1);
			if (field == "magFilter") magFilter = (TextureFilterMode)state->Peek<int>(-1);
			if (field == "wrap") wrap = (TextureWrapMode)state->Peek<int>(-1);
			if (field == "mipmaps") mipmaps = state->Peek<bool>(-1);
		});
		if (!mipmaps && minFilter == TextureFilterMode::LinearMipMapNearest) minFilter = TextureFilterMode::Linear;
		TextureBase* tex = nullptr;
		switch (type) {
		case TextureType::Single2D:
			tex = new Texture2D({ size.x, size.y }, mipmaps, format);
			break;
		case TextureType::Array2D:
			tex = new Texture2DArray({ size.x, size.y, size.z }, mipmaps, format);
			break;
		case TextureType::Single3D:
			tex = new Texture3D({ size.x, size.y, size.z }, mipmaps, format);
			break;
		case TextureType::SingleCubeMap:
			tex = new TextureCube({ size.x, size.x }, mipmaps, format);
			break;
		}
		if (tex != nullptr) {
			tex->SetName(name);
			tex->Bind();
			tex->bSetMinFilter(minFilter);
			tex->bSetMagFilter(magFilter);
			tex->bSetWrap(wrap);
			tex->Unbind();
			m_material->SetTexture(name, tex);
			m_textures.emplace_back(tex);
			state->Push<void*>(tex);
		}
		fs::current_path(cdir);
		return tex == nullptr ? 0 : 1;
	}

	int RenderPipeline::LinkTexture(Lua::State * state)
	{
		TextureType type = (TextureType)state->Peek<int>(1);
		string name = state->Peek<string>(2);
		TextureFormat format = TextureFormat::RGBA8;
		TextureFilterMode minFilter = TextureFilterMode::LinearMipMapNearest;
		TextureFilterMode magFilter = TextureFilterMode::Linear;
		TextureWrapMode wrap = TextureWrapMode::ClampToEdge;
		state->PeekTable(3, [&](const string& field) {
			if (field == "minFilter") minFilter = (TextureFilterMode)state->Peek<int>(-1);
			if (field == "magFilter") magFilter = (TextureFilterMode)state->Peek<int>(-1);
			if (field == "wrap") wrap = (TextureWrapMode)state->Peek<int>(-1);
		});
		TextureBase* tex = nullptr;
		switch (type) {
		case TextureType::Single2D:
			tex = Application::GetResourceByName<Texture2D>(name).get();
			break;
		case TextureType::Array2D:
			tex = Application::GetResourceByName<Texture2DArray>(name).get();
			break;
		case TextureType::SingleCubeMap:
			tex = Application::GetResourceByName<TextureCube>(name).get();
			break;
		}
		if (tex != nullptr) {
			tex->SetName(name);
			tex->Bind();
			tex->bSetMinFilter(minFilter);
			tex->bSetMagFilter(magFilter);
			tex->bSetWrap(wrap);
			tex->Unbind();
			m_material->SetTexture(name, tex);
			state->Push<void*>(tex);
		}
		return tex == nullptr ? 0 : 1;
	}

	int RenderPipeline::CreateRenderTarget(Lua::State * state)
	{
		int width = state->Peek<int>(1);
		int height = state->Peek<int>(2);
		TextureFormat format = TextureFormat::RGBA8;
		TextureFilterMode minFilter = TextureFilterMode::LinearMipMapNearest;
		TextureFilterMode magFilter = TextureFilterMode::Linear;
		TextureWrapMode wrap = TextureWrapMode::ClampToEdge;
		bool mipmaps = false;
		bool depthBuffer = false;
		bool stencilBuffer = false;
		vector<string> colorBufs;
		state->PeekArray(3, [&](int index) {
			index--;
			if (index >= colorBufs.size()) colorBufs.resize(index + 1);
			colorBufs[index] = state->Peek<string>(-1);
		});
		state->PeekTable(4, [&](const string& field) {
			if (field == "format") format = (TextureFormat)state->Peek<int>(-1);
			if (field == "minFilter") minFilter = (TextureFilterMode)state->Peek<int>(-1);
			if (field == "magFilter") magFilter = (TextureFilterMode)state->Peek<int>(-1);
			if (field == "wrap") wrap = (TextureWrapMode)state->Peek<int>(-1);
			if (field == "mipmaps") mipmaps = state->Peek<bool>(-1);
			if (field == "depthBuffer") depthBuffer = state->Peek<bool>(-1);
			if (field == "stencilBuffer") stencilBuffer = state->Peek<bool>(-1);
		});
		if (!mipmaps && minFilter == TextureFilterMode::LinearMipMapNearest) minFilter = TextureFilterMode::Linear;
		RenderTarget* rt = new RenderTarget({ width, height }, colorBufs.size(), format, minFilter, magFilter, mipmaps, depthBuffer, stencilBuffer);
		for (int i = 0; i < colorBufs.size(); ++i) {
			(*rt)[i]->SetName(colorBufs[i]);
			m_material->SetTexture(colorBufs[i], (*rt)[i]);
		}
		m_renderTargets.emplace_back(rt);
		state->Push<void*>(rt);
		return 1;
	}

	int RenderPipeline::FindShader(Lua::State * state)
	{
		auto sh = Application::GetResourceByName<ShaderProgram>(state->Peek<string>(1));
		if (sh) {
			state->Push<void*>(sh.get());
			return 1;
		}
		return 0;
	}

	int RenderPipeline::Enable(Lua::State * state)
	{
		ogele::Enable((Feature)state->Peek<int>(1));
		return 0;
	}

	int RenderPipeline::Disable(Lua::State * state)
	{
		ogele::Disable((Feature)state->Peek<int>(1));
		return 0;
	}

	int RenderPipeline::BlendFunc(Lua::State * state)
	{
		ogele::BlendFunc((BlendFactor)state->Peek<int>(1), (BlendFactor)state->Peek<int>(2));
		return 0;
	}

	int RenderPipeline::DispatchCompute(Lua::State * state)
	{
		auto sh = static_cast<ShaderProgram*>(state->Peek<void*>(1));
		sh->Bind();
		Application::GetMaterial()->Apply(sh);
		m_material->Apply(sh);
		sh->bDispatchCompute({ state->Peek<int>(2), state->Peek<int>(3), state->Peek<int>(4) });
		sh->Unbind();
		return 0;
	}
	int RenderPipeline::DrawScreen(Lua::State * state)
	{
		ogele::Disable(Feature::CullFace);
		ogele::Disable(Feature::DepthTest);
		ogele::Disable(Feature::Blend);
		auto rt = static_cast<RenderTarget*>(state->Peek<void*>(1));
		auto sh = static_cast<ShaderProgram*>(state->Peek<void*>(2));
		rt->Bind();
		Viewport({ 0,0 }, (*rt)[0]->GetSize());
		sh->Bind();
		Application::GetMaterial()->Apply(sh);
		m_material->Apply(sh);
		if (m_cam != nullptr) m_cam->GetMaterial()->Apply(sh);
		Application::DrawScreenQuad();
		sh->Unbind();
		rt->Unbind();
		return 0;
	}

	void RenderPipeline::DrawRecursive(const Camera* cam, Transform* node, const std::vector<std::string>& actorTags, const std::vector<std::string>& shaderTags) {
		if (node->GetActor() != nullptr)
			if (actorTags.size() == 0 || node->GetActor()->HasTags(actorTags))
				for (const auto& c : node->GetActor()->GetComponents()) {
					if (Renderable* r = dynamic_cast<Renderable*>(c.get()))
						if (r->IsActive())
							r->Render(cam, shaderTags, m_material.get());
				}
		for (const auto& t : node->GetChilds())
			DrawRecursive(cam, t.get(), actorTags, shaderTags);
	}
	int RenderPipeline::DrawScene(Lua::State * state)
	{
		static vector<string> shaderTags;
		static vector<string> actorTags;
		shaderTags.clear();
		actorTags.clear();
		auto rt = static_cast<RenderTarget*>(state->Peek<void*>(1));
		state->PeekArray(2, [&](int index) { shaderTags.push_back(state->Peek<string>(-1)); });
		state->PeekArray(3, [&](int index) { actorTags.push_back(state->Peek<string>(-1)); });
		rt->Bind();
		Viewport({ 0,0 }, (*rt)[0]->GetSize());
		for (const auto& root : m_world->GetRoots())
			DrawRecursive(m_cam, root.get(), actorTags, shaderTags);
		rt->Unbind();
		return 0;
	}
	int RenderPipeline::SetFinalRenderTarget(Lua::State * state)
	{
		m_finalTarget = static_cast<RenderTarget*>(state->Peek<void*>(1));
		m_finalTargetTextureIndex = state->Peek<int>(2);
		return 0;
	}
	int RenderPipeline::ClearRenderTarget(Lua::State * state)
	{
		BufferBit bufs{};
		vec4 clearColor{};
		auto rt = static_cast<RenderTarget*>(state->Peek<void*>(1));
		state->PeekArray(2, [&](int index) { bufs = bufs | (BufferBit)state->Peek<int>(-1); });
		state->PeekArray(3, [&](int index) { clearColor[index - 1] = state->Peek<float>(-1); });
		rt->Bind();
		ClearColor(clearColor);
		Clear(bufs);
		rt->Unbind();
		return 0;
	}
	int RenderPipeline::ResizeRenderTarget(Lua::State * state)
	{
		auto rt = static_cast<RenderTarget*>(state->Peek<void*>(1));
		rt->Resize({ state->Peek<int>(2),state->Peek<int>(3) });
		return 0;
	}
}