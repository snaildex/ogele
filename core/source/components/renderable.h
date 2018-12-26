#pragma once
namespace ogele {
	class Camera;
	class Renderable : public Component {
	public:
		virtual ~Renderable() = default;
		virtual void Render(const Camera* camera, const std::vector<std::string>& tags) const = 0;
	};
}