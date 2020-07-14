#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	void Application::DrawLine(const std::vector<dvec3>& coords, const vec3& color)
	{
		if (coords.size() < 2) return;
		if (m_instance->m_debugCamera == nullptr) return;
		for (int i = 0; i < coords.size() - 1; ++i) {
			m_instance->m_lines->PushVertex(coords[i], color);
			m_instance->m_lines->PushVertex(coords[i + 1], color);
		}
	}
	void Application::DrawRay(const dvec3& pos, const dvec3& dir, const vec3& color)
	{
		if (m_instance->m_debugCamera == nullptr) return;
		double l = length(dir);
		l = (l - 0.1) / l;
		dvec3 side = cross(normalize(dir), normalize(pos+dir- m_instance->m_debugCamera->GetTransform()->GetPos())) * 0.05;
		m_instance->m_lines->PushVertex(pos, color);
		m_instance->m_lines->PushVertex(pos + dir, color);
		m_instance->m_triangles->PushVertex(pos + dir, color);
		m_instance->m_triangles->PushVertex(pos + dir * l + side, color);
		m_instance->m_triangles->PushVertex(pos + dir * l - side, color);
	}
	void Application::DrawTriangle(const std::array<glm::dvec3, 3>& pos, const vec3& color)
	{
		if (m_instance->m_debugCamera == nullptr) return;
		for (const vec3& p : pos) m_instance->m_triangles->PushVertex(p, color);
	}
	void Application::DrawCircle(const glm::dvec3& pos, const glm::dvec3& radius, const glm::dvec3& dir, int segments, const vec3& color)
	{
		if (m_instance->m_debugCamera == nullptr) return;
		dvec3 r2 = normalize(cross(cross(dir, radius), radius)) * length(radius);
		for (int i = 0; i < segments; ++i) {
			double a1 = 2 * M_PI / segments * i;
			double a2 = 2 * M_PI / segments * (i + 1);
			dvec3 p1 = pos + cos(a1) * radius + sin(a1) * r2;
			dvec3 p2 = pos + cos(a2) * radius + sin(a2) * r2;
			m_instance->m_lines->PushVertex(p1, color);
			m_instance->m_lines->PushVertex(p2, color);
		}
	}
}
