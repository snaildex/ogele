//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_COORDBASISMESH_H
#define OGELE_COORDBASISMESH_H
namespace ogele {
    class CoordBasisMesh : public MeshBase {
    public:
        struct Vertex {
            vec3 Position;
            vec3 Color;
        };
    private:
        unique_ptr<FloatBuffer<Vertex>> m_vertexBuffer;
    public:
        CoordBasisMesh();
    };
}
#endif //OGELE_COORDBASISMESH_H
