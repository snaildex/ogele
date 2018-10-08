//
// Created by ??????? on 24.08.2018.
//

#include <ogele.h>

namespace ogele {

	Clouds::Clouds(const ivec3 &resolution, const ivec2& frameSize) {
		m_resolution = resolution;
		m_density = make_unique<Texture3D>(resolution, false, TextureFormat::R16F);
		m_density->Bind();
		m_density->bSetMinFilter(TextureFilterMode::Linear);
		m_density->bSetMagFilter(TextureFilterMode::Linear);
		m_density->bSetWrap(TextureWrapMode::Repeat);
		m_density->Unbind();
		m_noise = make_unique<Texture3D>(m_resolution, false, TextureFormat::R16F);
		m_noise->Bind();
		m_noise->bSetMinFilter(TextureFilterMode::Linear);
		m_noise->bSetMagFilter(TextureFilterMode::Linear);
		m_noise->bSetWrap(TextureWrapMode::Repeat);
		m_noise->Unbind();
		m_material = make_unique<Material>();
		m_material->SetTexture("CloudDensityMap", m_density.get());
		m_material->SetTexture("CloudNoiseMap", m_noise.get());
		m_cloudsGen = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>("GenClouds");
		m_render = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>("RenderClouds");
		m_cloudNoiseGen = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>("GenCloudNoise");
		m_cloudNoiseGen->Bind();
		m_material->Apply(m_cloudNoiseGen);
		m_cloudNoiseGen->bDispatchCompute(m_resolution / ivec3(32, 1, 32));
		m_cloudNoiseGen->Unbind();
		m_screenQuad = Application::GetInstance()->GetResources()->GetResourceByName<ScreenQuadMesh>("ScreenQuad");
		for (int i = 0; i < 2; i++) {
			auto rt = new RenderTarget(frameSize, 1, TextureFormat::RGBA16F);
			auto tex = (*rt)[0];
			tex->Bind();
			tex->bSetMinFilter(TextureFilterMode::Linear);
			tex->bSetMagFilter(TextureFilterMode::Linear);
			tex->bSetWrap(TextureWrapMode::ClampToEdge);
			tex->Unbind();
			m_buffer.add(rt);
		}
	}

	void Clouds::SetFrameSize(const ivec2& size) {
		for (int i = 0; i < m_buffer.size(); i++)
			m_buffer.reset(new RenderTarget(size, 1, TextureFormat::RGBA16F), i);
	}


	void Clouds::Generate() {
		m_cloudsGen->Bind();
		m_material->Apply(m_cloudsGen);
		m_cloudsGen->bDispatchCompute(m_resolution / ivec3(32, 1, 32));
		m_cloudsGen->Unbind();
	}

	void Clouds::Render(const Camera* cam, const vec3& sunDir) {
		trmat4 VP = cam->GetViewProjMatrix();
		trmat4 IVP = inverse(VP);
		m_buffer.swap();
		m_buffer->Bind();
		m_render->Bind();
		m_material->Apply(m_render);
		m_render->Set<float>("Time", Application::GetInstance()->GetTime());
		m_render->SetTexture("prevFrame", (*m_buffer[1])[0]);
		m_render->Set("IVP", IVP);
		m_render->Set("PrevVP", m_prevVP);
		m_render->Set("sunDir", sunDir);
		m_screenQuad->Draw();
		m_render->Unbind();
		m_buffer->Unbind();
		
		m_prevVP = VP;
	}
}