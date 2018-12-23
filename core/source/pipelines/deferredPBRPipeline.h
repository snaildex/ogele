#pragma once
namespace ogele {

	class DeferredPBRPipeline {
		Entity(DeferredPBRPipeline)
	private:
		std::unique_ptr<RenderTarget> m_GBufFBO;
		std::unique_ptr<RenderTarget> m_FinalFBO;
		std::array<std::unique_ptr<RenderTarget>, 2> m_RawFBO;
		Camera *m_cam;
		res_ptr<ShaderProgram> m_lightCompute;
		res_ptr<ShaderProgram> m_tonemap;
		res_ptr<ShaderProgram> m_skyboxGen;
		res_ptr<Texture2D> m_brdflut;
		std::unique_ptr<TextureCube> m_skybox;
		std::unique_ptr<Clouds> m_clouds;
		glm::ivec2 m_frameSize;
		int m_cloudsDownsample;
		glm::dvec3 m_sunDir;

	public:
		DeferredPBRPipeline(const glm::ivec2 &frameSize);

		 RenderTarget *GetGBuffer() const noexcept { return m_GBufFBO.get(); }

		 RenderTarget *GetFinalBuffer() const noexcept { return m_FinalFBO.get(); }

		 Camera *GetFrameCamera() const noexcept { return m_cam; }

		 void SetFrameCamera(Camera *cam) noexcept { m_cam = cam; }

		 const glm::dvec3 &GetSunDir() const noexcept { return m_sunDir; }

		 void SetSunDir(const glm::dvec3 &sunDir) noexcept { m_sunDir = sunDir; }

		void Resize(const glm::ivec2 &size);

		 void Bind() const { m_GBufFBO->Bind(); }

		void Unbind();
	};
}