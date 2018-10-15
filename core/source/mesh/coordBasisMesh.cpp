#include <mesh/coordBasisMesh.h>
#include <other/enums.h>

using namespace std;
using namespace glm;

namespace ogele {
    CoordBasisMesh::CoordBasisMesh() : MeshBase(Topology::Lines) {
        Vertex data[6]{
                {vec3(0, 0, 0), vec3(1, 0, 0)},
                {vec3(1, 0, 0), vec3(1, 0, 0)},
                {vec3(0, 0, 0), vec3(0, 1, 0)},
                {vec3(0, 1, 0), vec3(0, 1, 0)},
                {vec3(0, 0, 0), vec3(0, 0, 1)},
                {vec3(0, 0, 1), vec3(0, 0, 1)}
        };
        SetCount(6);
        m_vertexBuffer = make_unique<FloatBuffer<Vertex>>(BufferFlags::None, 6, data);
        SetBuffer(0, 3, m_vertexBuffer.get(), 0);
        SetBuffer(1, 3, m_vertexBuffer.get(), 3);
    }
}
