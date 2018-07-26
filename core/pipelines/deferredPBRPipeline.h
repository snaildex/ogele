//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_DEFERREDPBRPIPELINE_H
#define OGELE_DEFERREDPBRPIPELINE_H
namespace ogele {

    class DeferredPBRPipeline {
        unique_ptr<RenderTarget> m_GBufFBO;
        unique_ptr<RenderTarget> m_FinalFBO;
        unique_ptr<RenderTarget> m_RawFBO;
        Camera *m_cam;
        ShaderProgram *m_lightCompute;
        ShaderProgram *m_tonemap;
        ScreenQuadMesh *m_screenQuad;
        Texture2D *m_brdflut;
    public:
        DeferredPBRPipeline(const ivec2 &frameSize);

        inline RenderTarget *GetGBuffer() const noexcept { return m_GBufFBO.get(); }

        inline RenderTarget *GetFinalBuffer() const noexcept { return m_FinalFBO.get(); }

        inline Camera *GetFrameCamera() const noexcept { return m_cam; }

        inline void SetFrameCamera(Camera *cam) noexcept { m_cam = cam; }

        void Resize(const ivec2 &size);

        inline void Bind() const { m_GBufFBO->Bind(); }

        void Unbind();
    };
}
#endif //OGELE_DEFERREDPBRPIPELINE_H
