#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	const int MaxLineCount = 4096;
	const int ChunkLineCount = 64;

	void Application::InitLineDrawer()
	{
		m_debugCamera = nullptr;
		m_linesCoordsBuf = make_unique<DoubleBuffer<dvec3>>(BufferFlags::MapWrite, MaxLineCount, nullptr);
		m_linesColorsBuf = make_unique<FloatBuffer<vec3>>(BufferFlags::MapWrite, MaxLineCount, nullptr);
		m_linesVao = make_unique<VertexArray>(Topology::Lines);
		m_linesVao->Bind();
		m_linesVao->bAttachBuffer(0, 3, m_linesCoordsBuf.get(), 0);
		m_linesVao->bAttachBuffer(1, 3, m_linesColorsBuf.get(), 0);
		m_linesVao->Unbind();
	}
	void Application::LineDrawerStartFrame()
	{
		if (m_debugCamera == nullptr) return;
		m_linesCount = 0;
		m_currentLinesCount = 0;
		m_linesCoordsBuf->Bind();
		m_linesCoordsData = m_linesCoordsBuf->bMapRange(m_linesCount, ChunkLineCount, MappingOptionsFlags::InvalidateRange);
		m_linesCoordsBuf->Unbind();
		m_linesColorsBuf->Bind();
		m_linesColorsData = m_linesColorsBuf->bMapRange(m_linesCount, ChunkLineCount, MappingOptionsFlags::InvalidateRange);
		m_linesColorsBuf->Unbind();
	}
	void Application::PushLineVertex(const dvec3& coord, const vec3& color)
	{
		if (m_linesCount == MaxLineCount) return;
		if (m_currentLinesCount == ChunkLineCount) {
			m_currentLinesCount = 0;
			m_linesCoordsBuf->Bind();
			m_linesCoordsBuf->bUnmap();
			m_linesCoordsData = m_linesCoordsBuf->bMapRange(m_linesCount, ChunkLineCount, MappingOptionsFlags::InvalidateRange);
			m_linesCoordsBuf->Unbind();
			m_linesColorsBuf->Bind();
			m_linesColorsBuf->bUnmap();
			m_linesColorsData = m_linesColorsBuf->bMapRange(m_linesCount, ChunkLineCount, MappingOptionsFlags::InvalidateRange);
			m_linesColorsBuf->Unbind();
		}
		m_linesCoordsData[m_currentLinesCount] = coord;
		m_linesColorsData[m_currentLinesCount] = color;
		m_currentLinesCount++;
		m_linesCount++;
	}
	void Application::DrawLine(const std::vector<dvec3>& coords, const vec3& color)
	{
		if (m_instance->m_debugCamera == nullptr) return;
		for (int i = 0; i < coords.size() - 1; ++i) {
			m_instance->PushLineVertex(coords[i], color);
			m_instance->PushLineVertex(coords[i + 1], color);
		}
	}
	void Application::DrawRay(const dvec3& pos, const dvec3& dir, const vec3& color)
	{
		if (m_instance->m_debugCamera == nullptr) return;
		double l = length(dir);
		l = (l - 0.2) / l;
		dvec3 side = cross(normalize(dir), m_instance->m_debugCamera->GetTransform()->Forward()) * 0.1;
		m_instance->PushLineVertex(pos, color);
		m_instance->PushLineVertex(pos + dir, color);
		m_instance->PushLineVertex(pos + dir, color);
		m_instance->PushLineVertex(pos + dir * l + side, color);
		m_instance->PushLineVertex(pos + dir, color);
		m_instance->PushLineVertex(pos + dir * l - side, color);
	}
	void Application::LineDrawerEndFrame()
	{
		if (m_debugCamera == nullptr) return;
		m_linesCoordsBuf->Bind();
		m_linesCoordsBuf->bUnmap();
		m_linesCoordsBuf->Unbind();
		m_linesColorsBuf->Bind();
		m_linesColorsBuf->bUnmap();
		m_linesColorsBuf->Unbind();
		Disable(Feature::DepthTest);
		Disable(Feature::CullFace);
		m_linesVao->Bind();
		m_drawLines->Bind();
		m_drawLines->Set("MVP", m_debugCamera->GetViewProjMatrix());
		m_linesVao->bDraw(0, m_linesCount);
		m_linesVao->Unbind();
		m_drawLines->Unbind();
	}
	void Application::ShutdownLineDrawer()
	{
		m_linesVao.reset();
		m_linesCoordsBuf.reset();
		m_linesColorsBuf.reset();
	}
}
