//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_DEFERREDPBRPIPELINE_H
#define OGELE_DEFERREDPBRPIPELINE_H
namespace ogele {

    class DeferredPBRPipeline {
        unique_ptr<RenderTarget> m_GBufFBO;
        unique_ptr<RenderTarget> m_FinalFBO;
        array<unique_ptr<RenderTarget>,2> m_RawFBO;
        Camera *m_cam;
        ShaderProgram *m_lightCompute;
        ShaderProgram *m_tonemap;
        ShaderProgram *m_skyboxGen;
        ScreenQuadMesh *m_screenQuad;
        Texture2D *m_brdflut;
        unique_ptr<TextureCube> m_skybox;
        unique_ptr<Clouds> m_clouds;
		ivec2 m_frameSize;

        vec3 m_sunDir;

    public:
        DeferredPBRPipeline(const ivec2 &frameSize);

        inline RenderTarget *GetGBuffer() const noexcept { return m_GBufFBO.get(); }

        inline RenderTarget *GetFinalBuffer() const noexcept { return m_FinalFBO.get(); }

        inline Camera *GetFrameCamera() const noexcept { return m_cam; }

        inline void SetFrameCamera(Camera *cam) noexcept { m_cam = cam; }

        inline const vec3 &GetSunDir() const noexcept { return m_sunDir; }

        inline void SetSunDir(const vec3 &sunDir) noexcept { m_sunDir = sunDir; }

        void Resize(const ivec2 &size);

        inline void Bind() const { m_GBufFBO->Bind(); }

        void Unbind();
    };
}
#endif //OGELE_DEFERREDPBRPIPELINE_H
