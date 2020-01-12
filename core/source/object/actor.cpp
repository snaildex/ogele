#include <stdafx.h>
#include <ogele.h>
namespace ogele {
	void Actor::Update()
	{
		for (const auto& c : m_components) if(c->IsActive()) c->Update();
	}

	void Actor::OnGui()
	{
		for (const auto& c : m_components) if (c->IsActive()) c->OnGui();
	}

	Actor * Actor::Clone(Transform* transform) const
	{
		Actor* res = new Actor();
		res->SetTransform(transform);
		transform->SetActor(res);
		res->CopyNameTagsFrom(this);
		for (const auto& c : m_components)
			if (Component* cc = c->Clone(res)) res->m_components.emplace_back(cc);
		return res;
	}
	void DeleteComponent(Component * comp)
	{
		comp->GetActor()->DeleteComponent(comp);
	}
	void Component::SetActive(bool value)
	{
		if (m_active == value) return;
		m_active = value;
		if (m_active) OnEnbale(); else OnDisale();
	}
}