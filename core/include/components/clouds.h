#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "../object/object.h"
#include "../helpers/helpers.h"
#include "../texture/texture3D/texture3D.h"
#include "../shader/material.h"
#include "../framebuffer/renderTarget.h"
#include "../shader/shaderProgram.h"
#include "../mesh/screenQuadMesh.h"
#include "camera.h"

namespace ogele {

    class Clouds : public Object {
	
		ivec3 m_noise1Resolution;
		ivec3 m_noise2Resolution;
		int m_resolution;

        std::unique_ptr<Texture3D> m_noise1;
		std::unique_ptr<Texture3D> m_noise2;
		std::unique_ptr<Material> m_material;
		swapchain<RenderTarget> m_buffer;
		ShaderProgram* m_cloudsGen;
		ShaderProgram* m_cloudNoiseGen;
		ShaderProgram* m_render;
		ScreenQuadMesh *m_screenQuad;
		glm::dmat4 m_prevVP;

    public:

        Clouds(int resolution, const glm::ivec2& frameSize);

        inline int GetResolution() const noexcept { return m_resolution; }
		inline Material* GetMaterial() const noexcept { return m_material.get(); }
		inline RenderTarget* GetFrame() const noexcept { return m_buffer.get(); }
		void SetFrameSize(const glm::ivec2& size);

        void Generate();
		void Render(const Camera* cam, const glm::dvec3& sunDir);
    };
}
