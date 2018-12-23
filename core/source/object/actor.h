#pragma once
namespace ogele {

	class Transform;
	class Component;

	class Actor : public Object {

		Transform* m_transform;
		std::vector<std::unique_ptr<Component>> m_components;

	public:

		void SetTransform(Transform* transform) { m_transform = transform; }
		Transform* GetTransform() const noexcept { return m_transform; }

		const std::vector<std::unique_ptr<Component>>& GetComponents() const noexcept { return m_components; }
		template <class T, typename ... Args>
		T* AddComponent(Args ... args) {
			T* c = new T(args ...);
			c->SetActor(this);
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
			std::remove_if(m_components.begin(), m_components.end(), [](const std::unique_ptr<Component>& cptr) {
				return (reinterpret_cast<T*>(cptr.get()) != nullptr);
			});
		}
	};

	class Component {
		Entity(Component);
		Actor* m_actor;

	public:

		Component() = default;
		virtual ~Component() = default;

		virtual std::string GetName() const = 0;

		Actor* GetActor() const noexcept { return m_actor; };
		void SetActor(Actor* actor) noexcept { m_actor = actor; }
		Transform* GetTransform() const noexcept { return m_actor->GetTransform(); }

	};

}