//
// Created by ??????? on 22.07.2018.
//

#include <ogele.h>

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    DeferredPBRPipeline::DeferredPBRPipeline(const ivec2 &frameSize) {
        m_GBufFBO = make_unique<RenderTarget>(frameSize, 4, TextureFormat::RGBA32F, false, true);
        m_FinalFBO = make_unique<RenderTarget>(frameSize, 1, TextureFormat::RGB8);
        for (int i = 0; i < 2; i++)
            m_RawFBO[i] = make_unique<RenderTarget>(frameSize, 1, TextureFormat::RGBA32F);
        m_lightCompute = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>(
                "LightingCompute");
        m_tonemap = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>(
                "Tonemapping");
        m_skyboxGen = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>(
                "Skybox");
        m_screenQuad = Application::GetInstance()->GetResources()->GetResourceByName<ScreenQuadMesh>("ScreenQuad");
        m_brdflut = Application::GetInstance()->GetResources()->GetResourceByName<Texture2D>("BRDFLUT");

        m_skybox = make_unique<TextureCube>(ivec2(512, 512), true, TextureFormat::RGBA32F);
        m_skybox->Bind();
        m_skybox->bSetMinFilter(TextureFilterMode::LinearMipMapLinear);
        m_skybox->bSetMagFilter(TextureFilterMode::Linear);
        m_skybox->bGenerateMipMap();
        m_skybox->Unbind();

        m_clouds=make_unique<Clouds>(ivec3(64,64,64));
    }

    void DeferredPBRPipeline::Resize(const ivec2 &size) {
        m_GBufFBO.reset(new RenderTarget(size, 4, TextureFormat::RGBA32F, false, true));
        m_FinalFBO.reset(new RenderTarget(size, 1, TextureFormat::RGB8));
        for (int i = 0; i < 2; i++)
            m_RawFBO[i].reset(new RenderTarget(size, 1, TextureFormat::RGBA32F));
    }

    void DeferredPBRPipeline::Unbind() {
        trmat4 VP = m_cam->GetViewProjMatrix();
        trmat4 IVP = inverse(VP);

        Disable(Feature::CullFace);
        Disable(Feature::DepthTest);
        Disable(Feature::Blend);

        m_skyboxGen->Bind();
        m_skyboxGen->Set("sunDir",m_sunDir);
        m_skyboxGen->SetTexture("Skybox", m_skybox.get());
        m_skyboxGen->bDispatchCompute({16, 16, 6});
        m_skyboxGen->Unbind();

        Barrier(MemoryBarriers::ShaderImageAccess);

        m_skybox->Bind();
        m_skybox->bGenerateMipMap();
        m_skybox->Unbind();

        m_clouds->Generate();
        Barrier(MemoryBarriers::ShaderImageAccess);

        m_RawFBO[0]->Bind();
        m_lightCompute->Bind();
        m_lightCompute->Set("IVP", IVP);
        m_lightCompute->Set("sunDir", m_sunDir);
        m_lightCompute->Set<float>("Time", Application::GetInstance()->GetTime());
        m_lightCompute->SetTexture("AlbedoRough", (*m_GBufFBO.get())[0]);
        m_lightCompute->SetTexture("PosDepth", (*m_GBufFBO.get())[1]);
        m_lightCompute->SetTexture("NormalMetal", (*m_GBufFBO.get())[2]);
        m_lightCompute->SetTexture("Emission", (*m_GBufFBO.get())[3]);
        m_lightCompute->SetTexture("BRDFLUT", m_brdflut);
        m_lightCompute->SetTexture("Skybox", m_skybox.get());
        m_clouds->GetMaterial()->Apply(m_lightCompute);
        m_screenQuad->Draw();
        m_lightCompute->Unbind();
        m_RawFBO[0]->Unbind();

        m_FinalFBO->Bind();
        m_tonemap->Bind();
        m_tonemap->SetTexture("Frame", (*m_RawFBO[0].get())[0]);
        m_screenQuad->Draw();
        m_tonemap->Unbind();
        m_FinalFBO->Unbind();
    }
}
