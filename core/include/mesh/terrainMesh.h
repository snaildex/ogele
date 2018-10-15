#pragma once
#include <memory>
#include <array>

#include <glm/glm.hpp>

#include "../buffer/vertexBuffer.h"
#include "meshBase.h"

namespace ogele {
    class TerrainMesh : public MeshBase {
        std::unique_ptr <IntBuffer<glm::ivec2>> m_coord;
		std::unique_ptr <FloatBuffer<glm::vec2>> m_uv;
		std::unique_ptr <UnsignedIntBuffer<unsigned int>> m_indexes;
        int m_size;
    public:
        TerrainMesh(int size);

        inline int GetSize() const noexcept { return m_size; }
    };
}