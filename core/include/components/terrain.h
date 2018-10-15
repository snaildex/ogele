#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "../object/object.h"
#include "../helpers/helpers.h"
#include "../texture/texture3D/texture3D.h"
#include "../mesh/terrainMesh.h"
#include "../texture/texture2D/texture2D.h"
#include "../buffer/vertexBuffer.h"
#include "../shader/material.h"
#include "../shader/shaderProgram.h"
#include "camera.h"

namespace ogele {

    class Terrain : public Object {
        std::unique_ptr<TerrainMesh> m_plane;
		std::unique_ptr<Texture2D> m_heightmap;
		std::unique_ptr<Texture2D> m_normalmap;
		std::unique_ptr<IntBuffer<glm::ivec2>> m_offsets;
		std::unique_ptr<Material> m_mat;
        mutable std::vector<glm::ivec2> m_currentChunks;
        glm::ivec2 m_size;
		glm::dvec3 m_offset;
        int m_chunkSize;
        int m_drawRange;
        int m_lods;
        double m_lodDist;
        ShaderProgram *m_heightGen;
        ShaderProgram *m_normals;
        ShaderProgram *m_terrainDraw;
    public:
        inline TerrainMesh *GetPlaneMesh() const noexcept { return m_plane.get(); }

        inline const glm::ivec2 &GetSize() const noexcept { return m_size; }

        inline int GetChunkSize() const noexcept { return m_chunkSize; }

        inline int GetDrawRange() const noexcept { return m_drawRange; }

        inline void SetDrawRange(int range) noexcept { m_drawRange=range; }

        inline const glm::dvec3 &GetOffset() const noexcept { return m_offset; }

        void Generate();

        void Draw(Camera *cam) const;

        Terrain(const glm::ivec2 &size, int chunkSize);
    };
}