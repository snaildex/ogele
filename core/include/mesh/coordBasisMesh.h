#pragma once
#include <memory>

#include <glm/glm.hpp>

#include "../buffer/vertexBuffer.h"

#include "meshBase.h"

namespace ogele {
    class CoordBasisMesh : public MeshBase {
    public:
        struct Vertex {
            glm::vec3 Position;
			glm::vec3 Color;
        };
    private:
        std::unique_ptr<FloatBuffer<Vertex>> m_vertexBuffer;
    public:
        CoordBasisMesh();
    };
}
