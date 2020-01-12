#pragma once
namespace ogele {
	class ModelMesh : public MeshBase {
		std::unique_ptr<FloatBuffer <glm::vec4>> m_position;
		std::unique_ptr<FloatBuffer <glm::vec4>> m_normals;
		std::unique_ptr<FloatBuffer <glm::vec4>> m_tangents;
		std::unique_ptr<FloatBuffer <glm::vec4>> m_bitangents;
		std::array<std::unique_ptr<FloatBuffer <glm::vec4>>, 4> m_colors;
		std::array<std::unique_ptr<FloatBuffer<glm::vec4>>, 4> m_uvs;
		std::unique_ptr<UnsignedIntBuffer < unsigned int>> m_indexes;
		ShaderProgramCollection m_shaders;

	public:
		ModelMesh(const std::vector<std::string>& shaderTags) : MeshBase(Topology::Triangles) { m_shaders.FindResouces(shaderTags); }

		FloatBuffer <glm::vec4> *GetPositionBuffer() const noexcept { return m_position.get(); }
		FloatBuffer <glm::vec4> *GetNormalsBuffer() const noexcept { return m_normals.get(); }
		FloatBuffer <glm::vec4> *GetTangentsBuffer() const noexcept { return m_tangents.get(); }
		FloatBuffer <glm::vec4> *GetBitangentsBuffer() const noexcept { return m_bitangents.get(); }
		FloatBuffer <glm::vec4> *GetColorBuffer(int num) const noexcept { return m_colors.at(num).get(); }
		FloatBuffer <glm::vec4> *GetUVBuffer(int num) const { return m_uvs.at(num).get(); }
		UnsignedIntBuffer <unsigned int> *GetIndexBuffer() const { return m_indexes.get(); }
		const ShaderProgramCollection& GetShaderProgramCollection() const noexcept { return m_shaders; }

		constexpr int GetUVBufferCount() const noexcept { return m_uvs.size(); }

		void SetPositionBuffer(FloatBuffer <glm::vec4> *buffer) {
			SetCount(buffer->GetSize() * 3);
			m_position.reset(buffer);
			SetBuffer(0, 3, buffer);
		}
		void SetNormalsBuffer(FloatBuffer <glm::vec4> *buffer) {
			m_normals.reset(buffer);
			SetBuffer(1, 3, buffer);
		}
		void SetTangentsBuffer(FloatBuffer <glm::vec4> *buffer) {
			m_tangents.reset(buffer);
			SetBuffer(2, 3, buffer);
		}
		void SetBitangentsBuffer(FloatBuffer <glm::vec4> *buffer) {
			m_bitangents.reset(buffer);
			SetBuffer(3, 3, buffer);
		}
		void SetColorBuffer(int num, FloatBuffer <glm::vec4> *buffer) {
			m_colors[num].reset(buffer);
			SetBuffer(4 + num, 4, buffer);
		}
		void SetUVBuffer(int num, FloatBuffer <glm::vec4> *buffer) {
			m_uvs[num].reset(buffer);
			SetBuffer(8 + num, 3, buffer);
		}

		void SetIndexBuffer(UnsignedIntBuffer<unsigned int> *buffer);
	};
}
