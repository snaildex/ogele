#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Model::Model(size_t materialCount, const std::vector<std::string>& shaderTags) {
		m_shaders = make_unique<ShaderProgramCollection>();
		m_shaders->FindResouces(shaderTags);
		m_root = make_unique<Transform>();
		m_materials.resize(materialCount);
	}

	void RenderNode(ShaderProgram* shader, const Camera* cam, const dmat4& M, const dmat4& VP, Transform* node) {
		Actor* act = node->GetActor();
		if (act)
			if (auto mnode = act->GetComponent<Model::Node>()) {
				cam->GetMaterial()->Apply(shader);
				Application::GetMaterial()->Apply(shader);
				shader->Set("MVP", VP*M*node->GetMatrix());
				for (size_t i = 0; i < mnode->GetMeshCount(); ++i)
					mnode->GetMesh(i)->Draw();
			}
		for (const auto& cnode : node->GetChilds())
			RenderNode(shader, cam, M, VP, cnode.get());
	}

	void Model::Render(const Camera* cam, const std::vector<std::string>& tags, const Transform* transform) const {
		auto shader = m_shaders->Get(tags);
		dmat4 M = transform == nullptr ? dmat4() : transform->GetMatrix();
		dmat4 VP = cam->GetViewProjMatrix();
		shader->Bind();
		RenderNode(shader.get(), cam, M, VP, m_root.get());
		shader->Unbind();
	}

	void TransformGUI(Transform* transform) {
		
	}

	void Model::GUI() const {

		TransformGUI(m_root.get());
	}
}