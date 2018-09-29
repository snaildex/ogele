//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_TERRAINMESH_H
#define OGELE_TERRAINMESH_H
namespace ogele {
    class TerrainMesh : public MeshBase {
        unique_ptr <IntBuffer<ivec2>> m_coord;
        unique_ptr <FloatBuffer<vec2>> m_uv;
        unique_ptr <UnsignedIntBuffer<unsigned int>> m_indexes;
        int m_size;
    public:
        TerrainMesh(int size);

        inline int GetSize() const noexcept { return m_size; }
    };
}
#endif //OGELE_TERRAINMESH_H
