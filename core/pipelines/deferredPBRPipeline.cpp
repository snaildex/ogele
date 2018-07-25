//
// Created by ??????? on 22.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    DeferredPBRPipeline::DeferredPBRPipeline(const ivec2 &frameSize) {
        m_GBufFBO = make_unique<RenderTarget>(frameSize, 4, TextureFormat::RGBA32F, false, true);
        m_FinalFBO = make_unique<RenderTarget>(frameSize, 1, TextureFormat::RGB8);
        m_lightCompute = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>(
                "LightingCompute");
        m_screenQuad = Application::GetInstance()->GetResources()->GetResourceByName<ScreenQuadMesh>("ScreenQuad");
        m_brdflut = Application::GetInstance()->GetResources()->GetResourceByName<Texture2D>("BRDFLUT");
    }

    void DeferredPBRPipeline::Resize(const ivec2 &size) {
        m_GBufFBO.reset(new RenderTarget(size, 4, TextureFormat::RGBA32F, false, true));
        m_FinalFBO.reset(new RenderTarget(size, 1, TextureFormat::RGB8));
    }

    void DeferredPBRPipeline::Unbind() {
        trmat4 VP = m_cam->GetViewProjMatrix();
        trmat4 IVP = inverse(VP);

        Disable(Feature::CullFace);
        Disable(Feature::DepthTest);
        Disable(Feature::Blend);

        m_FinalFBO->Bind();
        m_lightCompute->Bind();

        m_lightCompute->Set("IVP", IVP);

        m_lightCompute->SetTexture("AlbedoRough", (*m_GBufFBO.get())[0]);
        m_lightCompute->SetTexture("PosDepth", (*m_GBufFBO.get())[1]);
        m_lightCompute->SetTexture("NormalMetal", (*m_GBufFBO.get())[2]);
        m_lightCompute->SetTexture("Emission", (*m_GBufFBO.get())[3]);
        m_lightCompute->SetTexture("BRDFLUT", m_brdflut);
        //m_lightCompute->Set("CamPos", m_cam->GetLocalPos());

        m_screenQuad->Draw();
        m_lightCompute->Unbind();
        m_FinalFBO->Unbind();
    }
}
