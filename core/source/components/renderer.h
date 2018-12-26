#pragma once
namespace ogele {
	class Renderer : public Renderable {
		res_ptr<Model> m_model;
	public:
		Renderer() = default;
		virtual ~Renderer() = default;
		virtual void Render(const Camera* camera, const std::vector<std::string>& tags) const override;
		virtual std::string GetName() const override { return "Renderer"; };
		void SetModel(res_ptr<Model> model) noexcept { m_model = model; }
		res_ptr<Model> GetModel() const noexcept { return m_model; }
	};
}