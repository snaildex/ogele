#pragma once
namespace ogele {

	class Sparks : public Renderable {
		ShaderProgramCollection m_shaders;
		static ShaderProgram* m_generator;
		std::unique_ptr<GridMesh3D> m_mesh;
		std::unique_ptr<Material> m_material;
		int m_range;
		double m_interval;

	public:
		Sparks(Actor* actor, int range);
		void SetSprites(res_ptr<Texture2DArray> value) noexcept;
		void SetColor(const vec4& color);
		void SetInterval(double value);
		void SetSize(double min, double max);
		virtual std::string GetName() const override { return "Spark system"; }
		virtual void Render(const Camera* camera, const std::vector<std::string>& tags, const ogele::Material* material = nullptr) const override;
	};

}