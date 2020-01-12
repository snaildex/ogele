#pragma once
namespace ogele {

	class World;

	class Transform {
		glm::dvec3 m_localPos;
		glm::dquat m_localRot;
		glm::dmat4 m_matrix;
		glm::dmat4 m_matrixInv;
		glm::dmat4 m_localMatrix;
		glm::dvec3 m_localScale;

		void UpdateMatrix();

		void UpdateMatrixRecursive();

		World* m_world;
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
		void SetLocalScale(const glm::dvec3 &value) noexcept {
			m_localScale = value;
			UpdateMatrix();
		}
		void SerLocalPosRot(const glm::dvec3 &pos, const glm::dquat &rot) {
			m_localPos = pos;
			m_localRot = rot;
			UpdateMatrix();
		}
		void SerLocalPosRotScale(const glm::dvec3 &pos, const glm::dquat &rot, const glm::dvec3 &scale) {
			m_localPos = pos;
			m_localRot = rot;
			m_localScale = scale;
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

		void SetWorld(World* world) noexcept;
		World* GetWorld() const noexcept { return m_world; }

		const glm::dvec3 &GetLocalPos() const noexcept { return m_localPos; }
		const glm::dquat &GetLocalRot() const noexcept { return m_localRot; }
		const glm::dvec3 &GetLocalScale() const noexcept { return m_localScale; }
		glm::dvec3 GetLocalEulerAngles() const noexcept { return eulerAngles(m_localRot); }
		const glm::dmat4 &GetMatrix() const noexcept { return m_matrix; }
		const glm::dmat4 &GetLocalMatrix() const noexcept { return m_localMatrix; }
		glm::dvec3 GetPos() const noexcept { return m_matrix * glm::dvec4(0, 0, 0, 1); }
		glm::dquat GetRot() const noexcept { return glm::quat_cast(m_matrix * glm::toMat4(m_localRot)); }
		glm::dvec3 GetEulerAngles() const noexcept { return eulerAngles(GetRot()); }

		glm::dvec3 Forward() const { return m_matrix * glm::dvec4(0, 0, -1, 0); }
		glm::dvec3 Right() const { return m_matrix * glm::dvec4(1, 0, 0, 0); }
		glm::dvec3 Up() const { return m_matrix * glm::dvec4(0, 1, 0, 0); }

		glm::dvec3 ToWorld(const glm::dvec3 &pos) const noexcept { return m_matrix * glm::dvec4(pos, 1); }
		glm::dvec3 ToLocal(const glm::dvec3 &pos) const noexcept { return m_matrixInv * glm::dvec4(pos, 1); }

		void MoveLocal(const dvec3& pos) {
			m_localPos += pos;
			UpdateMatrix();
		}
		void Rotate(const dquat& rot) {
			m_localRot *= rot;
			UpdateMatrix();
		}
		void Rotate(const dvec3& eulers) {
			m_localRot *= dquat(eulers);
			UpdateMatrix();
		}
		void LookAt(const glm::dvec3 &pos, const glm::dvec3 &up = { 0, 1, 0 }) noexcept {
			SetLocalRot(LookAtRotation(glm::normalize(pos - m_localPos), up));
		}
		void Look(const glm::dvec3 &dir, const glm::dvec3 &up = { 0, 1, 0 }) noexcept {
			SetLocalRot(LookAtRotation(dir, up));
		}

		const std::list<std::unique_ptr<Transform>>& GetChilds() const noexcept { return m_childs; }

		Transform *CreateChild();
		Actor* CreateChildActor(const std::string& name) { return CreateChild()->CreateActor(name); }
		Actor* CreateChildActor(const std::string& name, const std::initializer_list<std::string>& tags) {
			return CreateChild()->CreateActor(name, tags);
		}

		Transform* Clone(Transform* parent) const;

		void ForEach(std::function<void(Transform*)> action);
		Transform* FindChild(std::function<bool(Transform*)> predicate);
		std::vector<Transform*> FindChilds(std::function<bool(Transform*)> predicate);

		void SetParent(Transform *parent);
	};
}
