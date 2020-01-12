#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Model::Model(size_t materialCount) {
		m_root = make_unique<Transform>();
		m_materials.resize(materialCount);
	}

	Actor * Model::Instantiate() const
	{
		return m_root->Clone(nullptr)->GetActor();
	}

	void TransformGUI(Transform* transform) {

	}

	void Model::GUI() const {

		TransformGUI(m_root.get());
	}

	Component* Model::Node::Clone(Actor* actor) const {
		Node* res = new Node(actor);
		res->m_meshes = m_meshes;
		res->m_shaders = m_shaders;
		return res;
	}
	void Model::Node::FindShaders(const std::vector<std::string>& tags)
	{
		m_shaders.FindResouces(tags);
	}
	void Model::Node::Render(const Camera * camera, const std::vector<std::string>& tags, const ogele::Material* material) const
	{
		auto VP = camera->GetViewProjMatrix();
		auto M = GetTransform()->GetMatrix();
		auto shader = m_shaders.Get(tags);
		shader->Bind();
		camera->GetMaterial()->Apply(shader.get());
		Application::GetMaterial()->Apply(shader.get());
		if (material) material->Apply(shader.get());
		shader->Set("MVP", VP*M);
		shader->Set("M", M);
		for (size_t i = 0; i < GetMeshCount(); ++i)
			GetMesh(i)->Draw();
		shader->Unbind();
	}

}