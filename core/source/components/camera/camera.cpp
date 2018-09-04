//
// Created by ??????? on 22.07.2018.
//

#include <ogele.h>

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    Camera::Camera(const ivec2 &frameSize) {
        m_frameSize = frameSize;
    }

    void PerspectiveCamera::UpdateProjection() {
        m_projMatrix = perspectiveFov(m_fov, static_cast<trscalar>(GetFrameSize().x),
                                      static_cast<trscalar>(GetFrameSize().y), m_zNear, m_zFar);
        m_viewProjMatrix = m_projMatrix * m_viewMatrix;
    }

    void PerspectiveCamera::UpdateView() {
        m_viewMatrix = inverse(GetMatrix());
        m_viewProjMatrix = m_projMatrix * m_viewMatrix;
    }

    void PerspectiveCamera::OnTransformUpdate() {
        UpdateView();
    }

    void PerspectiveCamera::OnFrameSizeChanged() {
        UpdateProjection();
    }

    PerspectiveCamera::PerspectiveCamera(const ivec2 &frameSize, trscalar fov, trscalar zNear, trscalar zFar) :
            Camera(frameSize) {
        m_fov = fov;
        m_zNear = zNear;
        m_zFar = zFar;
        UpdateProjection();
        UpdateView();
    }

    void Camera::LookAround(const dvec2 &delta, trscalar speed, trscalar xMin, trscalar xMax) {
        m_eulerAngles.y -= delta.x * speed;
        m_eulerAngles.x -= delta.y * speed;
        m_eulerAngles.x = glm::clamp(m_eulerAngles.x, xMin, xMax);
        SetLocalEulerAngles(m_eulerAngles);
    }
}
