//
// Created by ??????? on 22.07.2018.
//

#include <ogele.h>

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {
    TerrainMesh::TerrainMesh(int size) : MeshBase(Topology::Patches) {
        m_size = size;
        int count = size * size;
        int elemCount = (size - 1) * (size - 1) * 4;
        SetCount(elemCount);
        unique_ptr<unsigned int[]> indexes(new unsigned int[elemCount]);
        unique_ptr<ivec2[]> coords(new ivec2[count]);
        unique_ptr<vec2[]> uvs(new vec2[count]);
        int i = 0;
        for (int x = 0; x < size - 1; x++)
            for (int y = 0; y < size - 1; y++) {
                indexes[i++] = y * size + x + 1;
                indexes[i++] = (y + 1) * size + x + 1;
                indexes[i++] = (y + 1) * size + x;
                indexes[i++] = (y) * size + x;
            }
        for (int x = 0; x < size; x++)
            for (int y = 0; y < size; y++) {
                coords[x + y * size] = {x, y};
                uvs[x + y * size] = {((float) x) / (size - 1), ((float) y) / (size - 1)};
            }
        m_indexes.reset(new UnsignedIntBuffer<unsigned int>(BufferFlags::None, elemCount, indexes.get()));
        m_coord.reset(new IntBuffer<ivec2>(BufferFlags::None, count, coords.get()));
        m_uv.reset(new FloatBuffer<vec2>(BufferFlags::None, count, uvs.get()));
        GetVAO()->Bind();
        GetVAO()->bAttachElementBuffer(m_indexes.get());
        GetVAO()->bAttachBuffer(0, 2, m_coord.get());
        GetVAO()->bAttachBuffer(1, 2, m_uv.get());
        GetVAO()->Unbind();
        GetVAO()->SetPatchVertCount(4);
    }
}
