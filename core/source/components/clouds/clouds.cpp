//
// Created by ??????? on 24.08.2018.
//

#include <ogele.h>

namespace ogele {

    Clouds::Clouds(const ivec3 &resolution) {
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
        m_material=make_unique<Material>();
        m_material->SetTexture("CloudDensityMap", m_density.get());
        m_material->SetTexture("CloudNoiseMap", m_noise.get());
        m_cloudsGen = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>("GenClouds");
        m_cloudNoiseGen = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>("GenCloudNoise");
        m_cloudNoiseGen->Bind();
        m_material->Apply(m_cloudNoiseGen);
        m_cloudNoiseGen->bDispatchCompute(m_resolution/ivec3(32,1,32));
        m_cloudNoiseGen->Unbind();
    }

    void Clouds::Generate() {
        m_cloudsGen->Bind();
        m_material->Apply(m_cloudsGen);
        m_cloudsGen->bDispatchCompute(m_resolution/ivec3(32,1,32));
        m_cloudsGen->Unbind();
    }
}