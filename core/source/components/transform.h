#pragma once
namespace ogele {

	class Transform {
		glm::dvec3 m_localPos;
		glm::dquat m_localRot;
		glm::dmat4 m_matrix;
		glm::dmat4 m_matrixInv;
		glm::dmat4 m_localMatrix;

		void UpdateMatrix();

		void UpdateMatrixRecursive();

		Transform *m_parent;
		std::unique_ptr<Actor> m_actor;
		std::list<std::unique_ptr<Transform>> m_childs;
	protected:
		virtual void OnTransformUpdate() {};
	public:
		Transform();

		void SetLocalPos(const glm::dvec3 &value) noexcept {
			m_localPos = value;
			UpdateMatrix();
		}

		void SetLocalRot(const glm::dquat &value) noexcept {
			m_localRot = value;
			UpdateMatrix();
		}

		void SerLocalPosRot(const glm::dvec3 &pos, const glm::dquat &rot) {
			m_localPos = pos;
			m_localRot = rot;
			UpdateMatrix();
		}

		void SetLocalEulerAngles(const glm::dvec3 &value) noexcept {
			m_localRot = glm::dquat(value);
			m_localRot = glm::normalize(m_localRot);
			UpdateMatrix();
		}

		void SetActor(Actor* actor) noexcept { actor->SetTransform(this); m_actor.reset(actor); }
		Actor* CreateActor(const std::string& name);
		Actor* CreateActor(const std::string& name, const std::initializer_list<std::string>& tags);
		Actor* GetActor() const noexcept { return m_actor.get(); }
		Actor* ReleaseActor() noexcept { return m_actor.release(); }

		const glm::dvec3 &GetLocalPos() const noexcept { return m_localPos; }
		const glm::dquat &GetLocalRot() const noexcept { return m_localRot; }
		glm::dvec3 GetLocalEulerAngles() const noexcept { return eulerAngles(m_localRot); }
		const glm::dmat4 &GetMatrix() const noexcept { return m_matrix; }
		const glm::dmat4 &GetLocalMatrix() const noexcept { return m_localMatrix; }
		glm::dvec3 GetPos() const noexcept { return m_matrix * glm::dvec4(m_localPos, 1); }
		glm::dquat GetRot() const noexcept { return glm::quat_cast(m_matrix * glm::toMat4(m_localRot)); }
		glm::dvec3 GetEulerAngles() const noexcept { return eulerAngles(GetRot()); }

		glm::dvec3 Forward() const { return m_matrix * glm::dvec4(0, 0, -1, 0); }
		glm::dvec3 Right() const { return m_matrix * glm::dvec4(1, 0, 0, 0); }
		glm::dvec3 Up() const { return m_matrix * glm::dvec4(0, 1, 0, 0); }

		glm::dvec3 ToWorld(const glm::dvec3 &pos) const noexcept { return m_matrix * glm::dvec4(pos, 1); }
		glm::dvec3 ToLocal(const glm::dvec3 &pos) const noexcept { return m_matrixInv * glm::dvec4(pos, 1); }

		void LookAt(const glm::dvec3 &pos, const glm::dvec3 &up = { 0, 1, 0 }) noexcept {
			SetLocalRot(LookAtRotation(pos - m_localPos, up));
		}

		const std::list<std::unique_ptr<Transform>>& GetChilds() const noexcept { return m_childs; }

		Transform *CreateChild();
		Actor* CreateChildActor(const std::string& name) { return CreateChild()->CreateActor(name); }
		Actor* CreateChildActor(const std::string& name, const std::initializer_list<std::string>& tags) {
			return CreateChild()->CreateActor(name, tags);
		}

		void SetParent(Transform *parent);
	};
}
