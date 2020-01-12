#pragma once
namespace ogele {

    class Clouds : public Object {

		ivec3 m_noise1Resolution;
		ivec3 m_noise2Resolution;
		int m_resolution;

        std::unique_ptr<Texture3D> m_noise1;
		std::unique_ptr<Texture3D> m_noise2;
		std::unique_ptr<Material> m_material;
		swapchain<RenderTarget> m_buffer;
		res_ptr<ShaderProgram> m_cloudsGen;
		res_ptr<ShaderProgram> m_cloudNoiseGen;
		res_ptr<ShaderProgram> m_render;
		glm::dmat4 m_prevVP;

    public:

        Clouds(int resolution, const glm::ivec2& frameSize);

         int GetResolution() const noexcept { return m_resolution; }
		 Material* GetMaterial() const noexcept { return m_material.get(); }
		 RenderTarget* GetFrame() const noexcept { return m_buffer.get(); }
		void SetFrameSize(const glm::ivec2& size);

        void Generate();
		void Render(const Camera* cam, const glm::dvec3& sunDir, TextureCube* skybox);
    };
}
