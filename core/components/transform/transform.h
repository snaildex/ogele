//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_TRANSFORM_H
#define OGELE_TRANSFORM_H
namespace ogele {

    class Transform : public Object {
        trvec3 m_localPos;
        trquat m_localRot;
        trmat4 m_matrix;
        trmat4 m_matrixInv;
        trmat4 m_localMatrix;

        void UpdateMatrix();

        void UpdateMatrixRecursive();

        Transform *m_parent;
        list<unique_ptr<Transform>> m_childs;
    protected:
        virtual void OnTransformUpdate() {};
    public:
        Transform();

        inline void SetLocalPos(const trvec3 &value) noexcept {
            m_localPos = value;
            UpdateMatrix();
        }

        inline void SetLocalRot(const trquat &value) noexcept {
            m_localRot = value;
            UpdateMatrix();
        }

        inline void SetLocalEulerAngles(const trvec3 &value) noexcept {
            m_localRot = trquat(value);
            m_localRot = normalize(m_localRot);
            UpdateMatrix();
        }

        inline const trvec3 &GetLocalPos() const noexcept { return m_localPos; }

        inline const trquat &GetLocalRot() const noexcept { return m_localRot; }

        inline const trvec3 GetLocalEulerAngles() const noexcept { return eulerAngles(m_localRot); }

        inline const trmat4 &GetMatrix() const noexcept { return m_matrix; }

        inline trvec3 Forward() const { return m_matrix * vec4(0, 0, -1, 0); }

        inline trvec3 Right() const { return m_matrix * vec4(1, 0, 0, 0); }

        inline trvec3 Up() const { return m_matrix * vec4(0, 1, 0, 0); }

        inline trvec3 ToWorld(const trvec3 &pos) const noexcept { return m_matrix * trvec4(pos, 1); }

        inline trvec3 ToLocal(const trvec3 &pos) const noexcept { return m_matrixInv * trvec4(pos, 1); }

        inline void LookAt(const trvec3 &pos, const trvec3 &up = {0, 1, 0}) noexcept {
            SetLocalRot(LookAtRotation(pos - m_localPos, up));
        }

        Transform *CreateChild();

        void SetParent(Transform *parent);
    };
}
#endif //OGELE_TRANSFORM_H
