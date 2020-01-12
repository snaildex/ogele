#pragma once
namespace ogele {
	class Model : public Resource {
	public:
		class Proxy;
		class Node : public Renderable {
			std::vector<ModelMesh*> m_meshes;
			ShaderProgramCollection m_shaders;
		public:
			Node(Actor* actor) :Renderable(actor) {};
			virtual std::string GetName() const override { return "Mesh node"; };
			virtual Component* Clone(Actor* actor) const override;
			void AddMesh(ModelMesh* mesh) noexcept { m_meshes.push_back(mesh); }
			size_t GetMeshCount() const noexcept { return m_meshes.size(); }
			ModelMesh* GetMesh(size_t num) const { return m_meshes[num]; }
			void FindShaders(const std::vector<std::string>& tags);
			virtual void Render(const Camera* camera, const std::vector<std::string>& tags, const ogele::Material* material = nullptr) const override;
		};

	private:
		std::vector<std::unique_ptr<Material>> m_materials;
		std::vector<std::pair<std::unique_ptr<ModelMesh>, size_t>> m_meshes;
		std::unique_ptr<Transform> m_root;

	public:
		Model(size_t materialCount);
		Transform* GetRoot() const noexcept { return m_root.get(); };
		Actor* Instantiate() const;
		size_t GetMeshCount() const noexcept { return m_meshes.size(); }
		ModelMesh* GetMesh(size_t num) const { return m_meshes[num].first.get(); }
		void AddMesh(ModelMesh* mesh, size_t materialId) noexcept { m_meshes.emplace_back(std::unique_ptr<ModelMesh>(mesh), materialId); }
		size_t GetMaterialCount() const noexcept { return m_materials.size(); }
		Material* GetMaterial(size_t num) const { return m_materials[num].get(); }
		void SetMaterial(size_t num, Material* material) { return m_materials[num].reset(material); }
		void GUI() const;
	};
}