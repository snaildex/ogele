#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Sparks::Sparks(Actor * actor, int range) : Renderable(actor)
	{
		GetActor()->AddTag("Sparks");
		m_shaders.FindResouces({ "Sparks" });
		m_material = make_unique<Material>();
		m_mesh = make_unique<GridMesh3D>(range);
		m_range = range;
		SetColor({ 1, 1, 1, 1 });
		SetSize(0.04, 0.20);
		SetInterval(8);
	}
	void Sparks::SetSprites(res_ptr<Texture2DArray> value) noexcept
	{
		m_material->SetTexture("Sprites", value.get());
	}
	void Sparks::SetColor(const vec4 & color)
	{
		m_material->Set("Color", color);
	}
	void Sparks::SetInterval(double value)
	{
		m_interval = value;
		m_material->Set("Interval", value);
		m_material->Set("Range", m_range*m_interval);
	}
	void Sparks::SetSize(double min, double max)
	{
		m_material->Set("MinSize", min);
		m_material->Set("MaxSize", max);
	}
	void Sparks::Render(const Camera * camera, const std::vector<std::string>& tags, const ogele::Material* material) const
	{
		dvec3 pos = floor(camera->GetTransform()->GetPos() / m_interval) * m_interval;
		auto sh = m_shaders.Get(tags);
		sh->Bind();
		Application::GetMaterial()->Apply(sh.get());
		camera->GetMaterial()->Apply(sh.get());
		if (material) material->Apply(sh.get());
		m_material->Apply(sh.get());
		sh->Set("Center", pos);
		m_mesh->Draw();
		sh->Unbind();
	}

}