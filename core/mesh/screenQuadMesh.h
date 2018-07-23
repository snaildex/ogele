//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_SCREENQUADMESH_H
#define OGELE_SCREENQUADMESH_H
namespace ogele {
    class ScreenQuadMesh : public MeshBase {
    public:
        struct Vertex {
            vec2 UV;
            vec3 NearPosition;
            vec3 FarPosition;
        };
    private:
        unique_ptr <FloatBuffer<Vertex>> m_vertexBuffer;
    public:
        ScreenQuadMesh();
    };
}
#endif //OGELE_SCREENQUADMESH_H
