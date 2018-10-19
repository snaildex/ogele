#include <mesh/gridMesh.h>
#include <other/enums.h>
#include <vector>

using namespace std;
using namespace glm;

namespace ogele {
	GridMesh::GridMesh(int range) : MeshBase(Topology::Points) {
		m_range = range;
		vector<ivec2> coords;
		for (int x = -range; x <= range; x++)
			for (int y = -range; y <= range; y++)
				if (length(vec2(x, y)) <= range)
					coords.emplace_back(x, y);
		SetCount(coords.size());
		m_coord=make_unique<IntBuffer<ivec2>>(BufferFlags::None, coords.size(), coords.data());
		SetBuffer(0,2, m_coord.get());
	}
}
