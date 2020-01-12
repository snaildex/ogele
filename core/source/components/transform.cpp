#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {

	void Transform::UpdateMatrix() {
		m_localMatrix = translate(dmat4(), m_localPos) * mat4_cast(m_localRot) * scale(dmat4(), m_localScale);
		UpdateMatrixRecursive();
		OnTransformUpdate();
	}

	void Transform::UpdateMatrixRecursive() {
		m_matrix = m_parent ? m_parent->m_matrix * m_localMatrix : m_localMatrix;
		m_matrixInv = inverse(m_matrix);
		for (auto &tr : m_childs) {
			tr->UpdateMatrixRecursive();
			tr->OnTransformUpdate();
		}
	}

	Transform::Transform() {
		m_world = nullptr;
		m_parent = nullptr;
		m_localPos = { 0, 0, 0 };
		m_localRot = identity<dquat>();
		m_localScale = { 1, 1, 1 };
		UpdateMatrix();
	}

	Actor* Transform::CreateActor(const std::string& name) {
		Actor* a = new Actor();
		a->SetTransform(this);
		a->SetName(name);
		m_actor.reset(a);
		return a;
	}

	Actor* Transform::CreateActor(const std::string& name, const std::initializer_list<std::string>& tags) {
		Actor* a = new Actor();
		a->SetTransform(this);
		a->SetName(name);
		for (const auto& tag : tags)
			a->AddTag(tag);
		m_actor.reset(a);
		return a;
	}

	void Transform::SetWorld(World * world) noexcept
	{
		m_world = world;
		for (const auto& c : m_childs)
			c->SetWorld(world);
	}

	Transform *Transform::CreateChild() {
		Transform *res = new Transform();
		res->SetParent(this);
		return res;
	}

	Transform * Transform::Clone(Transform* parent) const
	{
		Transform* res = new Transform();
		if (m_actor != nullptr) m_actor->Clone(res);
		if (parent == nullptr && m_world != nullptr) m_world->AddRoot(res);
		res->SetParent(parent);
		res->SerLocalPosRotScale(m_localPos, m_localRot, m_localScale);
		res->SetWorld(m_world);
		for (const auto& c : m_childs) c->Clone(res);
		return res;
	}

	void ProcessTransformRecursive(Transform* tr, std::function<void(Transform*)> action) {
		action(tr);
		for (const auto& c : tr->GetChilds())
			ProcessTransformRecursive(c.get(), action);
	}

	void Transform::ForEach(std::function<void(Transform*)> action)
	{
		ProcessTransformRecursive(this, action);
	}

	Transform * Transform::FindChild(std::function<bool(Transform*)> predicate)
	{
		Transform* res = nullptr;
		ProcessTransformRecursive(this, [&](Transform* tr) { if (res == nullptr && predicate(tr)) res = tr; });
		return res;
	}

	std::vector<Transform *> Transform::FindChilds(std::function<bool(Transform*)> predicate)
	{
		std::vector<Transform*> res;
		ProcessTransformRecursive(this, [&](Transform* tr) { if (predicate(tr)) res.push_back(tr); });
		return res;
	}

	void Transform::SetParent(Transform *parent) {
		if (m_parent != nullptr)
			m_parent->m_childs.remove_if([this](const unique_ptr<Transform> &tr) { return tr.get() == this; });
		if (parent != nullptr) {
			parent->m_childs.emplace_back(this);
			m_world = parent->m_world;
		}
		m_parent = parent;
		UpdateMatrixRecursive();
		OnTransformUpdate();
	}
}
