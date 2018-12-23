#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Clouds::Clouds(int resolution, const ivec2& frameSize) {
		m_resolution = resolution;
		m_noise1Resolution = ivec3(resolution * 4, resolution / 16, resolution * 4);
		m_noise2Resolution = ivec3(resolution / 2 , resolution /2 , resolution / 2);
		m_noise1 = make_unique<Texture3D>(m_noise1Resolution, false, TextureFormat::R16F);
		m_noise1->Bind();
		m_noise1->bSetMinFilter(TextureFilterMode::Linear);
		m_noise1->bSetMagFilter(TextureFilterMode::Linear);
		m_noise1->bSetWrap(TextureWrapMode::Repeat);
		m_noise1->Unbind();
		m_noise2 = make_unique<Texture3D>(m_noise2Resolution, false, TextureFormat::R16F);
		m_noise2->Bind();
		m_noise2->bSetMinFilter(TextureFilterMode::Linear);
		m_noise2->bSetMagFilter(TextureFilterMode::Linear);
		m_noise2->bSetWrap(TextureWrapMode::Repeat);
		m_noise2->Unbind();
		m_material = make_unique<Material>();
		m_material->SetTexture("CloudDensityMap", m_noise1.get());
		m_material->SetTexture("CloudNoiseMap", m_noise2.get());
		m_cloudsGen = Application::GetResourceByName<ShaderProgram>("GenClouds");
		m_render = Application::GetResourceByName<ShaderProgram>("RenderClouds");
		m_cloudNoiseGen = Application::GetResourceByName<ShaderProgram>("GenCloudNoise");
		m_cloudNoiseGen->Bind();
		m_material->Apply(m_cloudNoiseGen.get());
		m_cloudNoiseGen->bDispatchCompute(m_noise2Resolution / ivec3(32, 1, 32));
		m_cloudNoiseGen->Unbind();
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
		m_material->Apply(m_cloudsGen.get());
		m_cloudsGen->bDispatchCompute(m_noise1Resolution / ivec3(32, 1, 32));
		m_cloudsGen->Unbind();
	}

	void Clouds::Render(const Camera* cam, const dvec3& sunDir, TextureCube* skybox) {
		dmat4 VP = cam->GetViewProjMatrix();
		dmat4 IVP = glm::inverse(VP);
		m_buffer.swap();
		m_buffer->Bind();
		m_render->Bind();
		m_material->Apply(m_render.get());
		m_render->SetTexture("Skybox", skybox);
		m_render->Set<float>("Time", Application::GetTime());
		m_render->SetTexture("prevFrame", (*m_buffer[1])[0]);
		m_render->Set("IVP", IVP);
		m_render->Set("PrevVP", m_prevVP);
		m_render->Set("sunDir", (vec3)sunDir);
		Application::DrawScreenQuad();
		m_render->Unbind();
		m_buffer->Unbind();

		m_prevVP = VP;
	}
}