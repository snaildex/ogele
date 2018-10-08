//
// Created by ??????? on 24.08.2018.
//

#ifndef OGELE_CLOUDS_H
#define OGELE_CLOUDS_H
namespace ogele {

    class Clouds : public Object {

        ivec3 m_resolution;

        unique_ptr<Texture3D> m_density;
        unique_ptr<Texture3D> m_noise;
        unique_ptr<Material> m_material;
		swapchain<RenderTarget> m_buffer;
		ShaderProgram* m_cloudsGen;
		ShaderProgram* m_cloudNoiseGen;
		ShaderProgram* m_render;
		ScreenQuadMesh *m_screenQuad;
		trmat4 m_prevVP;

    public:

        Clouds(const ivec3 &resolution, const ivec2& frameSize);

        inline const ivec3 &GetResolution() const noexcept { return m_resolution; }
		inline Material* GetMaterial() const noexcept { return m_material.get(); }
		inline RenderTarget* GetFrame() const noexcept { return m_buffer.get(); }
		void SetFrameSize(const ivec2& size);

        void Generate();
		void Render(const Camera* cam, const vec3& sunDir);
    };
}
#endif //OGELE_CLOUDS_H
