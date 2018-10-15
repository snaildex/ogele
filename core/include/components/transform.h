#pragma once
#include <memory>
#include <list>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../object/object.h"
#include "../helpers/math3D.h"


namespace ogele {

    class Transform : public Object {
		glm::dvec3 m_localPos;
		glm::dquat m_localRot;
		glm::dmat4 m_matrix;
		glm::dmat4 m_matrixInv;
		glm::dmat4 m_localMatrix;

        void UpdateMatrix();

        void UpdateMatrixRecursive();

        Transform *m_parent;
        std::list<std::unique_ptr<Transform>> m_childs;
    protected:
        virtual void OnTransformUpdate() {};
    public:
        Transform();

        inline void SetLocalPos(const glm::dvec3 &value) noexcept {
            m_localPos = value;
            UpdateMatrix();
        }

        inline void SetLocalRot(const glm::dquat &value) noexcept {
            m_localRot = value;
            UpdateMatrix();
        }

        inline void SetLocalEulerAngles(const glm::dvec3 &value) noexcept {
            m_localRot = glm::dquat(value);
            m_localRot = glm::normalize(m_localRot);
            UpdateMatrix();
        }

        inline const glm::dvec3 &GetLocalPos() const noexcept { return m_localPos; }

        inline const glm::dquat &GetLocalRot() const noexcept { return m_localRot; }

        inline const glm::dvec3 GetLocalEulerAngles() const noexcept { return eulerAngles(m_localRot); }

        inline const glm::dmat4 &GetMatrix() const noexcept { return m_matrix; }

        inline glm::dvec3 Forward() const { return m_matrix * glm::dvec4(0, 0, -1, 0); }

        inline glm::dvec3 Right() const { return m_matrix * glm::dvec4(1, 0, 0, 0); }

        inline glm::dvec3 Up() const { return m_matrix * glm::dvec4(0, 1, 0, 0); }

        inline glm::dvec3 ToWorld(const glm::dvec3 &pos) const noexcept { return m_matrix * glm::dvec4(pos, 1); }

        inline glm::dvec3 ToLocal(const glm::dvec3 &pos) const noexcept { return m_matrixInv * glm::dvec4(pos, 1); }

        inline void LookAt(const glm::dvec3 &pos, const glm::dvec3 &up = {0, 1, 0}) noexcept {
            SetLocalRot(LookAtRotation(pos - m_localPos, up));
        }

        Transform *CreateChild();

        void SetParent(Transform *parent);
    };
}
