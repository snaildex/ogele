#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {

	class MotionState :public btMotionState {
		Transform* m_tr;
	public:
		MotionState(Transform* transform) :m_tr(transform) {}

		void getWorldTransform(btTransform& worldTrans) const override {
			dvec3 pos = m_tr->GetPos();
			dquat rot = m_tr->GetRot();
			worldTrans.setOrigin({ pos.x,pos.y,pos.z });
			worldTrans.setRotation({ rot.x,rot.y,rot.z,rot.w });
		}

		void setWorldTransform(const btTransform& worldTrans) override {
			btVector3 pos = worldTrans.getOrigin();
			btQuaternion rot = worldTrans.getRotation();
			m_tr->SetLocalPos({ pos.x(),pos.y(),pos.z() });
			m_tr->SetLocalRot({ rot.w(),rot.x(),rot.y(),rot.z() });
		}
	};

	Rigidbody::Rigidbody(Actor* actor, double mass) : Component(actor)
	{
		MotionState* motionState = new MotionState(GetTransform());
		m_motionState = motionState;
		btCollisionShape* shape = new btSphereShape(1);
		btVector3 inertia;
		shape->calculateLocalInertia(mass, inertia);
		m_body.reset(new btRigidBody(mass, motionState, shape, inertia));
		m_body->setActivationState(DISABLE_DEACTIVATION);
		GetTransform()->GetWorld()->AddRigidbody(this);
	}

	Rigidbody::~Rigidbody()
	{
		delete ((MotionState*)m_motionState);
	}
}