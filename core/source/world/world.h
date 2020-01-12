#pragma once
namespace ogele {

	class World : public Object {
		std::list<std::unique_ptr<Transform>> m_roots;
		std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConf;
		std::unique_ptr<btCollisionDispatcher> m_collisionDispatcher;
		std::unique_ptr<btBroadphaseInterface> m_broadphaseInterface;
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_impulseSolver;
		std::unique_ptr<btDiscreteDynamicsWorld> m_world;

	public:
		World::World();
		const std::list<std::unique_ptr<Transform>>& GetRoots() const noexcept { return m_roots; }
		Transform* GetRoot(const std::string& name) const;
		Transform* CreateTransform() noexcept;
		Actor* CreateActor(const std::string& name) noexcept;

		void AddRoot(Transform* tr);
		void RemoveRoot(Transform* tr);

		void SetGravity(const dvec3& gravity);
		void StepPhysics(double deltaTime);
		void Update();
		void OnGui();
		void AddRigidbody(Rigidbody* rigidbody);
	};

}