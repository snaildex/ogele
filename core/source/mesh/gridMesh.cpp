#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	GridMesh2D::GridMesh2D(int range) : MeshBase(Topology::Points) {
		m_size = range;
		vector<ivec2> coords;
		for (int x = -range; x <= range; x++)
			for (int y = -range; y <= range; y++)
				if (length(vec2(x, y)) <= range)
					coords.emplace_back(x, y);
		SetCount(coords.size());
		m_coord=make_unique<IntBuffer<ivec2>>(BufferFlags::None, coords.size(), coords.data());
		SetBuffer(0,2, m_coord.get());
	}

	GridMesh3D::GridMesh3D(int range) : MeshBase(Topology::Points) {
		m_size = range;
		vector<ivec3> coords;
		for (int x = -range; x <= range; x++)
			for (int y = -range; y <= range; y++)
				for (int z = -range; z <= range; z++)
				if (length(vec3(x, y, z)) <= range)
					coords.emplace_back(x, y, z);
		SetCount(coords.size());
		m_coord = make_unique<IntBuffer<ivec3>>(BufferFlags::None, coords.size(), coords.data());
		SetBuffer(0, 3, m_coord.get());
	}
}
