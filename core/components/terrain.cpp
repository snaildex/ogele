//
// Created by ??????? on 22.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    Terrain::Terrain(const ivec2 &size, int chunkSize) {
        m_size = size;
        m_chunkSize = chunkSize;
        ivec2 totalSize = size * chunkSize;
        m_offset = {-totalSize.x / 2, 0, -totalSize.y / 2};
        m_plane = make_unique<TerrainMesh>(chunkSize);
        m_heightmap = make_unique<Texture2D>(totalSize, false, TextureFormat::R32F);
        m_normalmap = make_unique<Texture2D>(totalSize, false, TextureFormat::RGBA32F);
        m_heightGen = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>("TerrainGen");
        m_normals = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>("TerrainNormals");
        m_terrainDraw = Application::GetInstance()->GetResources()->GetResourceByName<ShaderProgram>("TerrainDraw");
        m_offsets = make_unique<IntBuffer<ivec2>>(BufferFlags::DynamicStorage, m_size.x * m_size.y, nullptr);
        m_drawRange = 3;
    }

    void Terrain::Generate() {
        ivec2 groupNum = m_chunkSize * m_size / 32;
        m_heightGen->Bind();
        m_heightGen->SetTexture("Heights", m_heightmap.get());
        m_heightGen->bDispatchCompute({groupNum.x, 1, groupNum.y});
        m_heightGen->Unbind();
        Barrier(MemoryBarriers::ShaderImageAccess);
        m_normals->Bind();
        m_normals->SetTexture("Heights", m_heightmap.get());
        m_normals->SetTexture("Normals", m_normalmap.get());
        m_normals->bDispatchCompute({groupNum.x, 1, groupNum.y});
        m_normals->Unbind();
    }

    void Terrain::Draw(Camera *cam) const {
        vec3 campos = (cam->GetLocalPos() - m_offset) / (float) m_chunkSize;
        vec2 camindex = {campos.x, campos.z};
        m_terrainDraw->Bind();
        m_terrainDraw->Set("VP", cam->GetViewProjMatrix());
        m_terrainDraw->SetTexture("Normals", m_normalmap.get());
        m_terrainDraw->SetTexture("Heights", m_heightmap.get());
        m_terrainDraw->Set("Offset", m_offset);
        m_terrainDraw->SetBuffer("Offsets", m_offsets.get());
        m_currentChunks.clear();
        for (int x = 0; x < m_size.x; x++)
            for (int y = 0; y < m_size.y; y++) {
                if (glm::distance(vec2(x, y), camindex) < m_drawRange) {
                    m_currentChunks.push_back(ivec2(x, y) * (m_chunkSize - 1));
                }
            }
        m_offsets->Bind();
        m_offsets->bSetData(m_currentChunks.data(), 0, m_currentChunks.size());
        m_offsets->Unbind();
        m_plane->Draw(m_currentChunks.size());
        m_terrainDraw->Unbind();
    }
}
