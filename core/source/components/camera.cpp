#include <components/camera.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

namespace ogele {

    Camera::Camera(const ivec2 &frameSize) {
        m_frameSize = frameSize;
    }

    void PerspectiveCamera::UpdateProjection() {
        m_projMatrix = glm::perspectiveFov(m_fov, static_cast<double>(GetFrameSize().x),
                                      static_cast<double>(GetFrameSize().y), m_zNear, m_zFar);
        m_viewProjMatrix = m_projMatrix * m_viewMatrix;
    }

    void PerspectiveCamera::UpdateView() {
        m_viewMatrix = glm::inverse(GetMatrix());
        m_viewProjMatrix = m_projMatrix * m_viewMatrix;
    }

    void PerspectiveCamera::OnTransformUpdate() {
        UpdateView();
    }

    void PerspectiveCamera::OnFrameSizeChanged() {
        UpdateProjection();
    }

    PerspectiveCamera::PerspectiveCamera(const ivec2 &frameSize, double fov, double zNear, double zFar) :
            Camera(frameSize) {
        m_fov = fov;
        m_zNear = zNear;
        m_zFar = zFar;
        UpdateProjection();
        UpdateView();
    }

    void Camera::LookAround(const dvec2 &delta, double speed, double xMin, double xMax) {
        m_eulerAngles.y -= delta.x * speed;
        m_eulerAngles.x -= delta.y * speed;
        m_eulerAngles.x = glm::clamp(m_eulerAngles.x, xMin, xMax);
        SetLocalEulerAngles(m_eulerAngles);
    }
}
