#pragma once
namespace ogele {
	class Camera;
	class Renderable : public Component {
	public:
		Renderable(Actor* actor) :Component(actor) {}
		virtual ~Renderable() = default;
		virtual void Render(const Camera* camera, const std::vector<std::string>& tags, const ogele::Material* material = nullptr) const = 0;
	};
}