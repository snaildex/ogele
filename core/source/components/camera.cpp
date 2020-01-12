#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Camera::Camera(Actor* actor, const ivec2 &frameSize) : Component(actor) {
		m_frameSize = frameSize;
		m_material = make_unique<Material>();
	}

	void Camera::UpdateMaterial() {
		dmat4 V = GetViewMatrix();
		dmat4 VP = GetViewProjMatrix();
		m_material->Set("P", m_projMatrix);
		m_material->Set("V", V);
		m_material->Set("VP", VP);
		m_material->Set("IVP", inverse(VP));
		m_material->Set("CamPos", GetTransform()->GetPos());
		m_material->Set("CamDir", GetTransform()->Forward());
		m_material->Set("Depth", GetTransform()->Forward());
	}

	void PerspectiveCamera::UpdateProjection() {
		m_projMatrix = glm::perspectiveFov(
			m_fov,
			static_cast<double>(GetFrameSize().x),
			static_cast<double>(GetFrameSize().y),
			m_zNear,
			m_zFar);
	}

	PerspectiveCamera::PerspectiveCamera(Actor* actor, const ivec2 &frameSize, double fov, double zNear, double zFar) :
		Camera(actor, frameSize) {
		m_fov = fov;
		m_zNear = zNear;
		m_zFar = zFar;
		UpdateProjection();
	}

	Component * PerspectiveCamera::Clone(Actor * actor) const
	{
		return new PerspectiveCamera(actor, GetFrameSize(), m_fov, m_zFar, m_zFar);
	}

	void Camera::LookAround(const dvec2 &delta, double speed, double xMin, double xMax) {
		m_eulerAngles.y -= delta.x * speed;
		m_eulerAngles.x -= delta.y * speed;
		m_eulerAngles.x = glm::clamp(m_eulerAngles.x, xMin, xMax);
		GetTransform()->SetLocalEulerAngles(m_eulerAngles);
	}
}
