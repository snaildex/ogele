//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_CAMERA_H
#define OGELE_CAMERA_H
namespace ogele {

    class Camera : public Transform {
        vec3 m_eulerAngles;
        ivec2 m_frameSize;
    protected:
        trmat4 m_viewMatrix;
        trmat4 m_projMatrix;
        trmat4 m_viewProjMatrix;

        virtual void OnFrameSizeChanged() {};
    protected:
        Camera(const ivec2 &frameSize);

    public:
        void LookAround(const dvec2 &delta, trscalar speed, trscalar xMin, trscalar xMax);

        inline void SetFrameSize(const ivec2 &value) noexcept {
            m_frameSize = value;
            OnFrameSizeChanged();
        }

        inline const ivec2 &GetFrameSize() const noexcept { return m_frameSize; }

        inline const float GetAspectRatio() const noexcept { return m_frameSize.x / (float) m_frameSize.y; }

        inline const trmat4 &GetViewMatrix() const noexcept { return m_viewMatrix; }

        inline const trmat4 &GetProjMatrix() const noexcept { return m_projMatrix; }

        inline const trmat4 &GetViewProjMatrix() const noexcept { return m_viewProjMatrix; }
    };

    class PerspectiveCamera : public Camera {
        trscalar m_fov;
        trscalar m_zNear;
        trscalar m_zFar;

        void OnTransformUpdate() override;

        void OnFrameSizeChanged() override;

        void UpdateProjection();

        void UpdateView();

    public:
        void SetFOV(trscalar value) noexcept {
            m_fov = value;
            UpdateProjection();
        }

        inline trscalar GetFOV() const noexcept { return m_fov; }

        void SetZNear(trscalar value) noexcept {
            m_zNear = value;
            UpdateProjection();
        }

        inline trscalar GetZNear() const noexcept { return m_zNear; }

        void SetZFar(trscalar value) noexcept {
            m_zFar = value;
            UpdateProjection();
        }

        inline trscalar GetZFar() const noexcept { return m_zFar; }

        PerspectiveCamera(const ivec2 &frameSize, trscalar fov, trscalar zNear, trscalar zFar);
    };
}
#endif //OGELE_CAMERA_H
