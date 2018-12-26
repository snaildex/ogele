#pragma once
namespace ogele {
	class Model : public Resource {
	public:
		class Proxy;
		class Node : public Component {
			std::vector<MeshBase*> m_meshes;
		public:
			virtual std::string GetName() const override { return "Mesh node"; };
			void AddMesh(MeshBase* mesh) noexcept { m_meshes.push_back(mesh); }
			size_t GetMeshCount() const noexcept { return m_meshes.size(); }
			MeshBase* GetMesh(size_t num) const { return m_meshes[num]; }
		};

	private:
		std::unique_ptr<ShaderProgramCollection> m_shaders;
		std::vector<std::unique_ptr<Material>> m_materials;
		std::vector<std::pair<std::unique_ptr<MeshBase>, size_t>> m_meshes;
		std::unique_ptr<Transform> m_root;

	public:
		Model(size_t materialCount, const std::vector<std::string>& shaderTags);
		Transform* GetRoot() const noexcept { return m_root.get(); };
		size_t GetMeshCount() const noexcept { return m_meshes.size(); }
		MeshBase* GetMesh(size_t num) const { return m_meshes[num].first.get(); }
		void AddMesh(MeshBase* mesh, size_t materialId) noexcept { m_meshes.emplace_back(std::unique_ptr<MeshBase>(mesh), materialId); }
		size_t GetMaterialCount() const noexcept { return m_materials.size(); }
		Material* GetMaterial(size_t num) const { return m_materials[num].get(); }
		void SetMaterial(size_t num, Material* material) { return m_materials[num].reset(material); }
		void Render(const Camera* cam, const std::vector<std::string>& tags, const Transform* transform = nullptr) const;
		void GUI() const;
	};
}