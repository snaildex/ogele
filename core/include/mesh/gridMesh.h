#pragma once
#include <memory>
#include <array>

#include <glm/glm.hpp>

#include "../buffer/vertexBuffer.h"
#include "meshBase.h"

namespace ogele {
	class GridMesh : public MeshBase {
		std::unique_ptr <IntBuffer<glm::ivec2>> m_coord;
		int m_range;
	public:
		GridMesh(int range);

		inline int GetRange() const noexcept { return m_range; }
	};
}