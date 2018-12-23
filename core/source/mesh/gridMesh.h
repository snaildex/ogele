#pragma once
namespace ogele {
	class GridMesh : public MeshBase {
		std::unique_ptr <IntBuffer<glm::ivec2>> m_coord;
		int m_range;
	public:
		GridMesh(int range);

		 int GetRange() const noexcept { return m_range; }
	};
}