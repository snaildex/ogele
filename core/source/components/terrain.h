#pragma once
namespace ogele {

	class Terrain : public Renderable {
		std::unique_ptr<TerrainMesh> m_plane;
		std::unique_ptr<GridMesh> m_grid;
		std::unique_ptr<Texture2D> m_heightmap;
		std::unique_ptr<Texture2D> m_normalmap;
		std::unique_ptr<IntBuffer<glm::ivec2>> m_offsets;
		std::unique_ptr<Material> m_mat;
		mutable std::vector<glm::ivec2> m_currentChunks;
		glm::ivec2 m_size;
		glm::dvec3 m_offset;
		int m_chunkSize;
		int m_drawRange;
		int m_lods;
		int m_grassRange;
		double m_lodDist;
		res_ptr<ShaderProgram> m_heightGen;
		res_ptr<ShaderProgram> m_normals;
		ShaderProgramCollection m_shaders;
		res_ptr<Texture2DArray> m_terrainAlbedo;
		res_ptr<Texture2DArray> m_terrainRoughness;
		res_ptr<Texture2DArray> m_terrainNormal;
		res_ptr<Texture2DArray> m_terrainHeight;
		res_ptr<Texture2DArray> m_grass;
	public:

		TerrainMesh *GetPlaneMesh() const noexcept { return m_plane.get(); }

		virtual std::string GetName() const override { return "Terrain"; };

		const glm::ivec2 &GetSize() const noexcept { return m_size; }

		int GetChunkSize() const noexcept { return m_chunkSize; }

		int GetDrawRange() const noexcept { return m_drawRange; }

		void SetDrawRange(int range) noexcept { m_drawRange = range; }

		const glm::dvec3 &GetOffset() const noexcept { return m_offset; }

		void Generate();

		virtual void Render(Camera* camera, const std::vector<std::string>& tags) const override;

		Terrain(const glm::ivec2 &size, int chunkSize);
	};
}