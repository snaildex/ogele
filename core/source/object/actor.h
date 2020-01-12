#pragma once
namespace ogele {

	class Transform;
	class Component;

	class Actor : public Object {

		Transform* m_transform;
		std::list<std::unique_ptr<Component>> m_components;

	public:

		void SetTransform(Transform* transform) { m_transform = transform; }
		Transform* GetTransform() const noexcept { return m_transform; }
		void Update();
		void OnGui();
		Actor* Clone(Transform* transform) const;

		const std::list<std::unique_ptr<Component>>& GetComponents() const noexcept { return m_components; }
		template <class T, typename ... Args>
		T* AddComponent(Args ... args) {
			T* c = new T(this, args ...);
			m_components.emplace_back(c);
			return c;
		}
		template <class T>
		T* GetComponent() {
			for (const auto& c : m_components) {
				T* res = dynamic_cast<T*>(c.get());
				if (res != nullptr)
					return res;
			}
			return nullptr;
		}
		template <class T>
		T* DeleteComponent() {
			m_components.remove_if([](const std::unique_ptr<Component>& cptr) {
				return (reinterpret_cast<T*>(cptr.get()) != nullptr);
			});
		}

		void DeleteComponent(Component* component) {
			m_components.remove_if([component](const std::unique_ptr<Component>& cptr) {
				return (cptr.get() == component);
			});
		}
	};

	class Component {
		Entity(Component);
		Actor* m_actor;
		bool m_active;

	protected:
		virtual void OnEnbale() {}
		virtual void OnDisale() {}

	public:
		Component(Actor* actor) :m_actor(actor), m_active(true) {};
		virtual ~Component() = default;
		virtual Component* Clone(Actor* actor) const { throw std::logic_error("Component cloning is not implemented"); };

		virtual std::string GetName() const = 0;
		virtual void Update() {}
		virtual void OnGui() {}

		bool IsActive() const noexcept { return m_active; }
		void SetActive(bool value);

		Actor* GetActor() const noexcept { return m_actor; };
		Transform* GetTransform() const noexcept { return m_actor->GetTransform(); }
	};

	void DeleteComponent(Component* comp);

}