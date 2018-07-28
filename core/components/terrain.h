//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_TERRAIN_H
#define OGELE_TERRAIN_H
namespace ogele {

    class Terrain : public Object {
        unique_ptr<TerrainMesh> m_plane;
        unique_ptr<Texture2D> m_heightmap;
        unique_ptr<Texture2D> m_normalmap;
        unique_ptr<IntBuffer<ivec2>> m_offsets;
        mutable vector<ivec2> m_currentChunks;
        ivec2 m_size;
        trvec3 m_offset;
        int m_chunkSize;
        int m_drawRange;
        ShaderProgram *m_heightGen;
        ShaderProgram *m_normals;
        ShaderProgram *m_terrainDraw;
    public:
        inline TerrainMesh *GetPlaneMesh() const noexcept { return m_plane.get(); }

        inline const ivec2 &GetSize() const noexcept { return m_size; }

        inline int GetChunkSize() const noexcept { return m_chunkSize; }

        inline int GetDrawRange() const noexcept { return m_drawRange; }

        inline void SetDrawRange(int range) noexcept { m_drawRange=range; }

        inline const trvec3 &GetOffset() const noexcept { return m_offset; }

        void Generate();

        void Draw(Camera *cam) const;

        Terrain(const ivec2 &size, int chunkSize);
    };
}
#endif //OGELE_TERRAIN_H
