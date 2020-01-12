#pragma once
namespace ogele {
	class GridMesh2D : public MeshBase {
		std::unique_ptr <IntBuffer<glm::ivec2>> m_coord;
		int m_size;
	public:
		GridMesh2D(int range);
		int GetRange() const noexcept { return m_size; }
	};

	class GridMesh3D : public MeshBase {
		std::unique_ptr <IntBuffer<glm::ivec3>> m_coord;
		int m_size;
	public:
		GridMesh3D(int range);
		int GetRange() const noexcept { return m_size; }
	};
}