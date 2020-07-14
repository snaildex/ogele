#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	const int MaxVertexCount = 40*1024;
	const int ChunkVertexCount = 64;
	VertexAccumulator::VertexAccumulator(Topology topology) {
		m_coordsBuf = make_unique<DoubleBuffer<dvec3>>(BufferFlags::MapWrite, MaxVertexCount, nullptr);
		m_colorsBuf = make_unique<FloatBuffer<vec3>>(BufferFlags::MapWrite, MaxVertexCount, nullptr);
		m_vao = make_unique<VertexArray>(topology);
		m_vao->Bind();
		m_vao->bAttachBuffer(0, 3, m_coordsBuf.get(), 0);
		m_vao->bAttachBuffer(1, 3, m_colorsBuf.get(), 0);
		m_vao->Unbind();
	}
	void VertexAccumulator::StartFrame() {
		m_count = 0;
		m_currentCount = 0;
		m_coordsBuf->Bind();
		m_coordsData = m_coordsBuf->bMapRange(m_count, ChunkVertexCount, MappingOptionsFlags::InvalidateRange);
		m_coordsBuf->Unbind();
		m_colorsBuf->Bind();
		m_colorsData = m_colorsBuf->bMapRange(m_count, ChunkVertexCount, MappingOptionsFlags::InvalidateRange);
		m_colorsBuf->Unbind();
	}
	void VertexAccumulator::PushVertex(const dvec3& coord, const vec3& color) {
		if (m_count == MaxVertexCount) return;
		if (m_currentCount == ChunkVertexCount) {
			m_currentCount = 0;
			m_coordsBuf->Bind();
			m_coordsBuf->bUnmap();
			m_coordsData = m_coordsBuf->bMapRange(m_count, ChunkVertexCount, MappingOptionsFlags::InvalidateRange);
			m_coordsBuf->Unbind();
			m_colorsBuf->Bind();
			m_colorsBuf->bUnmap();
			m_colorsData = m_colorsBuf->bMapRange(m_count, ChunkVertexCount, MappingOptionsFlags::InvalidateRange);
			m_colorsBuf->Unbind();
		}
		m_coordsData[m_currentCount] = coord;
		m_colorsData[m_currentCount] = color;
		m_currentCount++;
		m_count++;
	}
	void VertexAccumulator::EndFrame() {
		m_coordsBuf->Bind();
		m_coordsBuf->bUnmap();
		m_coordsBuf->Unbind();
		m_colorsBuf->Bind();
		m_colorsBuf->bUnmap();
		m_colorsBuf->Unbind();
	}
	void VertexAccumulator::Draw() {
		m_vao->Bind();
		m_vao->bDraw(0, m_count);
		m_vao->Unbind();
	}
}
