#pragma once

#include <glm/glm.hpp>

#include "transform.h"

namespace ogele {

    class Camera : public Transform {
        glm::dvec3 m_eulerAngles;
		glm::ivec2 m_frameSize;
    protected:
		glm::dmat4 m_viewMatrix;
		glm::dmat4 m_projMatrix;
		glm::dmat4 m_viewProjMatrix;

        virtual void OnFrameSizeChanged() {};
    protected:
        Camera(const glm::ivec2 &frameSize);

    public:
        void LookAround(const glm::dvec2 &delta, double speed, double xMin, double xMax);

        inline void SetFrameSize(const glm::ivec2 &value) noexcept {
            m_frameSize = value;
            OnFrameSizeChanged();
        }

        inline const glm::ivec2 &GetFrameSize() const noexcept { return m_frameSize; }

        inline const float GetAspectRatio() const noexcept { return m_frameSize.x / (float) m_frameSize.y; }

        inline const glm::dmat4 &GetViewMatrix() const noexcept { return m_viewMatrix; }

        inline const glm::dmat4 &GetProjMatrix() const noexcept { return m_projMatrix; }

        inline const glm::dmat4 &GetViewProjMatrix() const noexcept { return m_viewProjMatrix; }
    };

    class PerspectiveCamera : public Camera {
        double m_fov;
		double m_zNear;
		double m_zFar;

        void OnTransformUpdate() override;

        void OnFrameSizeChanged() override;

        void UpdateProjection();

        void UpdateView();

    public:
        void SetFOV(double value) noexcept {
            m_fov = value;
            UpdateProjection();
        }

        inline double GetFOV() const noexcept { return m_fov; }

        void SetZNear(double value) noexcept {
            m_zNear = value;
            UpdateProjection();
        }

        inline double GetZNear() const noexcept { return m_zNear; }

        void SetZFar(double value) noexcept {
            m_zFar = value;
            UpdateProjection();
        }

        inline double GetZFar() const noexcept { return m_zFar; }

        PerspectiveCamera(const glm::ivec2 &frameSize, double fov, double zNear, double zFar);
    };
}
