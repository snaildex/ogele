#pragma once

#include <memory>
#include <array>

#include <glm/glm.hpp>

#include "../framebuffer/renderTarget.h"
#include "../shader/shaderProgram.h"
#include "../components/camera.h"
#include "../mesh/screenQuadMesh.h"
#include "../texture/textureCube/textureCube.h"
#include "../components/clouds.h"
#include "../helpers/helpers.h"

namespace ogele {

	class DeferredPBRPipeline {
		Entity(DeferredPBRPipeline)
	private:
		std::unique_ptr<RenderTarget> m_GBufFBO;
		std::unique_ptr<RenderTarget> m_FinalFBO;
		std::array<std::unique_ptr<RenderTarget>, 2> m_RawFBO;
		Camera *m_cam;
		ShaderProgram *m_lightCompute;
		ShaderProgram *m_tonemap;
		ShaderProgram *m_skyboxGen;
		ScreenQuadMesh *m_screenQuad;
		Texture2D *m_brdflut;
		std::unique_ptr<TextureCube> m_skybox;
		std::unique_ptr<Clouds> m_clouds;
		glm::ivec2 m_frameSize;
		int m_cloudsDownsample;
		glm::dvec3 m_sunDir;

	public:
		DeferredPBRPipeline(const glm::ivec2 &frameSize);

		inline RenderTarget *GetGBuffer() const noexcept { return m_GBufFBO.get(); }

		inline RenderTarget *GetFinalBuffer() const noexcept { return m_FinalFBO.get(); }

		inline Camera *GetFrameCamera() const noexcept { return m_cam; }

		inline void SetFrameCamera(Camera *cam) noexcept { m_cam = cam; }

		inline const glm::dvec3 &GetSunDir() const noexcept { return m_sunDir; }

		inline void SetSunDir(const glm::dvec3 &sunDir) noexcept { m_sunDir = sunDir; }

		void Resize(const glm::ivec2 &size);

		inline void Bind() const { m_GBufFBO->Bind(); }

		void Unbind();
	};
}