#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {

	World::World() {
		m_collisionConf = make_unique<btDefaultCollisionConfiguration>();
		m_collisionDispatcher = make_unique<btCollisionDispatcher>(m_collisionConf.get());
		m_broadphaseInterface.reset(new btDbvtBroadphase());
		m_impulseSolver = make_unique<btSequentialImpulseConstraintSolver>();
		m_world = make_unique<btDiscreteDynamicsWorld>(m_collisionDispatcher.get(), m_broadphaseInterface.get(), m_impulseSolver.get(), m_collisionConf.get());
	}

	Transform * World::GetRoot(const std::string & name) const
	{
		for (const auto& r : m_roots)
			if (r->GetActor() && r->GetActor()->GetName() == name) return r.get();
		return nullptr;
	}

	Transform* World::CreateTransform() noexcept {
		Transform* tr = new Transform();
		tr->SetWorld(this);
		m_roots.emplace_back(tr);
		return tr;
	}

	Actor* World::CreateActor(const std::string& name) noexcept {
		return CreateTransform()->CreateActor(name);
	}

	void World::AddRoot(Transform * tr)
	{
		for (const auto& r : m_roots)
			if (r.get() == tr) return;
		m_roots.emplace_back(tr);
		if (tr->GetWorld() != this) tr->SetWorld(this);
	}

	void World::RemoveRoot(Transform * tr)
	{
		m_roots.remove_if([&](const unique_ptr<Transform>& t) {return t.get() == tr; });
		if (tr->GetWorld() == this) tr->SetWorld(nullptr);
	}

	void World::SetGravity(const dvec3 & gravity)
	{
		m_world->setGravity({ gravity.x, gravity.y, gravity.z });
	}

	void World::StepPhysics(double deltaTime)
	{
		m_world->stepSimulation(deltaTime, 10);
	}

	void World::Update()
	{
		for (const auto& r : m_roots) r->ForEach([](Transform* tr) {if (Actor* act = tr->GetActor()) act->Update(); });
	}

	void World::OnGui()
	{
		for (const auto& r : m_roots) r->ForEach([](Transform* tr) {if (Actor* act = tr->GetActor()) act->OnGui(); });
	}

	void World::AddRigidbody(Rigidbody * rigidbody)
	{
		m_world->addRigidBody(rigidbody->m_body.get());
	}

}