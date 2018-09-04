//
// Created by ??????? on 22.07.2018.
//

#include <ogele.h>

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {
    ScreenQuadMesh::ScreenQuadMesh() : MeshBase(Topology::TriangleStrip) {
        Vertex data[4]{
                {vec2(0, 0), vec3(-1, -1, -1), vec3(-1, -1, 1)},
                {vec2(0, 1), vec3(-1, 1, -1),  vec3(-1, 1, 1)},
                {vec2(1, 0), vec3(1, -1, -1),  vec3(1, -1, 1)},
                {vec2(1, 1), vec3(1, 1, -1),   vec3(1, 1, 1)}
        };
        SetCount(4);
        m_vertexBuffer = make_unique<FloatBuffer<Vertex>>(BufferFlags::None, 4, data);
        SetBuffer(0, 2, m_vertexBuffer.get(), 0);
        SetBuffer(1, 3, m_vertexBuffer.get(), 2);
        SetBuffer(2, 3, m_vertexBuffer.get(), 5);
    }
}
