#pragma once
namespace ogele {
	class ScreenQuadMesh : public MeshBase {
	public:
		struct Vertex {
			glm::vec2 UV;
			glm::vec3 NearPosition;
			glm::vec3 FarPosition;
		};
	private:
		std::unique_ptr <FloatBuffer<Vertex>> m_vertexBuffer;
	public:
		ScreenQuadMesh();
	};
}