#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	dmat4 ones(1);

	void Transform::UpdateMatrix() {
		m_localMatrix = glm::translate(ones, m_localPos);
		m_localMatrix *= toMat4(m_localRot);
		UpdateMatrixRecursive();
		OnTransformUpdate();
	}

	void Transform::UpdateMatrixRecursive() {
		m_matrix = m_parent ? m_parent->m_matrix * m_localMatrix : m_localMatrix;
		m_matrixInv = transpose(m_matrix);
		for (auto &tr : m_childs)
			tr->UpdateMatrix();
	}

	Transform::Transform() {
		m_parent = nullptr;
		m_localPos = { 0, 0, 0 };
		m_localRot = dquat();
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

	Transform *Transform::CreateChild() {
		Transform *res = new Transform();
		m_childs.emplace_back(res);
		return res;
	}

	void Transform::SetParent(Transform *parent) {
		if (m_parent != nullptr)
			m_parent->m_childs.remove_if([this](const unique_ptr<Transform> &tr) { return tr.get() == this; });
		parent->m_childs.emplace_back(this);
		m_parent = parent;
	}
}
