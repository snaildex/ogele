#pragma once
namespace ogele {

	class Rigidbody : public Component {
	private:
		std::unique_ptr<btRigidBody> m_body;
		void* m_motionState;

	public:
		friend class World;

		Rigidbody::Rigidbody(Actor* actor, double mass);
		virtual ~Rigidbody();

		virtual std::string GetName() const override { return "Rigidbody"; };

		void SetFriction(const dvec3& friction) { m_body->setAnisotropicFriction({ friction.x, friction.y, friction.z }); }
		void SetRollFriction(double friction) { m_body->setRollingFriction(friction); }
		void SetSpinFriction(double friction) { m_body->setSpinningFriction(friction); }
		void SetDamping(double linear, double angular) { m_body->setDamping(linear, angular); }

		void ApplyCentralImpulse(const dvec3& impulse) { m_body->applyCentralImpulse({ impulse.x, impulse.y, impulse.z }); }
		void ApplyTorqueImpulse(const dvec3& impulse) { m_body->applyTorqueImpulse({ impulse.x, impulse.y, impulse.z }); }
		void ApplyTorqueImpulse(const dquat& impulse) { ApplyTorqueImpulse(degrees(eulerAngles(impulse))); }

		void ApplyCentralForce(const dvec3& force) { m_body->applyCentralForce({ force.x, force.y, force.z }); }
		void ApplyTorque(const dvec3& torque) { m_body->applyTorque({ torque.x, torque.y, torque.z }); }
		void ApplyTorque(const dvec3& axis, double angle) {
			btQuaternion q({ axis.x, axis.y, axis.z }, angle);
			double x, y, z;
			q.getEulerZYX(z, y, x);
			m_body->applyTorque({ x,y,z });
		}

		//dvec3 GetAngularSpeed() const { m_body-> }
	};

}