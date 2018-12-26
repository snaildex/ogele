#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	void Renderer::Render(const Camera* camera, const std::vector<std::string>& tags) const {
		if (m_model.get() == nullptr)
			return;
		m_model->Render(camera, tags, GetActor()->GetTransform());
	}
}