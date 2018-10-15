#pragma once

#include <memory>
#include <array>

#include <glm/glm.hpp>

#include "../buffer/vertexBuffer.h"
#include "../other/enums.h"
#include "meshBase.h"

namespace ogele {
	class ModelMesh : public MeshBase {
		std::unique_ptr<FloatBuffer <glm::vec3>> m_position;
		std::unique_ptr<FloatBuffer <glm::vec3>> m_normals;
		std::unique_ptr<FloatBuffer <glm::vec3>> m_tangents;
		std::unique_ptr<FloatBuffer <glm::vec4>> m_color;
		std::array<std::unique_ptr<FloatBuffer<glm::vec2>>, 2> m_uvs;
		std::unique_ptr<UnsignedIntBuffer < unsigned int>> m_indexes;
	public:
		ModelMesh() : MeshBase(Topology::Triangles) {}

		inline FloatBuffer <glm::vec3> *GetPositionBuffer() const noexcept { return m_position.get(); }

		inline FloatBuffer <glm::vec3> *GetNormalsBuffer() const noexcept { return m_normals.get(); }

		inline FloatBuffer <glm::vec3> *GetTangentsBuffer() const noexcept { return m_tangents.get(); }

		inline FloatBuffer <glm::vec4> *GetColorBuffer() const noexcept { return m_color.get(); }

		inline FloatBuffer <glm::vec2> *GetUVBuffer(int num) const { return m_uvs[num].get(); }

		inline constexpr int GetUVBufferCount() const noexcept { return m_uvs.size(); }

		inline void SetPositionBuffer(FloatBuffer <glm::vec3> *buffer) {
			SetCount(buffer->GetSize() * 3);
			m_position.reset(buffer);
			SetBuffer(0, 3, buffer);
		}

		inline void SetNormalsBuffer(FloatBuffer <glm::vec3> *buffer) {
			m_normals.reset(buffer);
			SetBuffer(1, 3, buffer);
		}

		inline void SetTangentsBuffer(FloatBuffer <glm::vec3> *buffer) {
			m_tangents.reset(buffer);
			SetBuffer(2, 3, buffer);
		}

		inline void SetColorBuffer(FloatBuffer <glm::vec4> *buffer) {
			m_color.reset(buffer);
			SetBuffer(3, 4, buffer);
		}

		inline void SetUVBuffer(int num, FloatBuffer <glm::vec2> *buffer) {
			m_uvs[num].reset(buffer);
			SetBuffer(4 + num, 2, buffer);
		}

		void SetIndexBuffer(UnsignedIntBuffer<unsigned int> *buffer);
	};
}
