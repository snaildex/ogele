#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Terrain::Terrain(const ivec2 &size, int chunkSize) {
		m_mat = make_unique<Material>();
		m_size = size;
		m_chunkSize = chunkSize;
		ivec2 totalSize = size * chunkSize;
		m_offset = { -totalSize.x / 2, 0, -totalSize.y / 2 };
		m_grassRange = 160;
		m_plane = make_unique<TerrainMesh>(chunkSize + 1);
		m_grid = make_unique<GridMesh>(m_grassRange);
		m_heightmap = make_unique<Texture2D>(totalSize, false, TextureFormat::R32F);
		m_heightmap->Bind();
		m_heightmap->bSetMagFilter(TextureFilterMode::Linear);
		m_heightmap->bSetMinFilter(TextureFilterMode::Linear);
		m_heightmap->Unbind();
		m_normalmap = make_unique<Texture2D>(totalSize, false, TextureFormat::RGBA32F);
		m_normalmap->Bind();
		m_normalmap->bSetMagFilter(TextureFilterMode::Linear);
		m_normalmap->bSetMinFilter(TextureFilterMode::Linear);
		m_normalmap->Unbind();
		m_heightGen = Application::GetResourceByName<ShaderProgram>("TerrainGen").async();
		m_normals = Application::GetResourceByName<ShaderProgram>("TerrainNormals").async();
		m_offsets = make_unique<IntBuffer<ivec2>>(BufferFlags::DynamicStorage, m_size.x * m_size.y, nullptr);
		m_drawRange = 3;
		m_lods = 4;
		m_lodDist = 50;

		m_shaders.FindResouces({ "Terrain" });

		m_mat->Set("Offset", m_offset);
		m_mat->Set<float>("MaxLOD", m_lods);
		m_mat->Set("LODDist", m_lodDist);
		m_mat->Set<int>("Step", std::pow(2, m_lods));
		m_mat->Set("MapSize", dvec2(totalSize));
		m_mat->SetTexture("Normals", m_normalmap.get());
		m_mat->SetTexture("Heights", m_heightmap.get());
		m_mat->SetBuffer("Offsets", m_offsets.get());

		m_terrainAlbedo = Application::GetResourceByName<Texture2DArray>("TerrainAlbedo").async();
		m_terrainRoughness = Application::GetResourceByName<Texture2DArray>("TerrainRoughness").async();
		m_terrainNormal = Application::GetResourceByName<Texture2DArray>("TerrainNormal").async();
		m_grass = Application::GetResourceByName<Texture2DArray>("Grass").async();
		m_mat->SetTexture("Albedo", m_terrainAlbedo.get());
		m_mat->SetTexture("Roughness", m_terrainRoughness.get());
		m_mat->SetTexture("Normal", m_terrainNormal.get());
		m_mat->SetTexture("Grass", m_grass.get());
	}

	void Terrain::Generate() {
		ivec2 groupNum = m_chunkSize * m_size / 32;
		m_heightGen->Bind();
		m_mat->Apply(m_heightGen.get());
		m_heightGen->bDispatchCompute({ groupNum.x, 1, groupNum.y });
		m_heightGen->Unbind();
		Barrier(MemoryBarriers::ShaderImageAccess);
		m_normals->Bind();
		m_mat->Apply(m_normals.get());
		m_normals->bDispatchCompute({ groupNum.x, 1, groupNum.y });
		m_normals->Unbind();
	}

	void Terrain::Render(const Camera* camera, const std::vector<std::string>& tags) const {
		res_ptr<ShaderProgram> draw = m_shaders.Get(tags);
		Enable(Feature::CullFace);
		int step = std::pow(2, m_lods);
		dvec3 campos = (camera->GetTransform()->GetLocalPos() - m_offset) / (double)(m_chunkSize * step);
		dvec2 camindex = { campos.x, campos.z };
		draw->Bind();
		m_mat->Apply(draw.get());
		draw->Set("VP", camera->GetViewProjMatrix());
		draw->Set("CamPos", camera->GetTransform()->GetLocalPos());
		m_currentChunks.clear();
		for (int x = 0; x < m_size.x / step; x++)
			for (int y = 0; y < m_size.y / step; y++)
				if (glm::distance(dvec2(x, y), camindex) < m_drawRange)
					m_currentChunks.push_back(ivec2(x, y) * (m_chunkSize * step));
		m_offsets->Bind();
		m_offsets->bSetData(m_currentChunks.data(), 0, m_currentChunks.size());
		m_offsets->Unbind();
		m_plane->Draw(m_currentChunks.size());
		draw->Unbind();

		/*m_grassDraw->Bind();
		m_grassDraw->Set("VP", cam->GetViewProjMatrix());
		m_grassDraw->Set("CamPos", cam->GetLocalPos());
		m_grassDraw->Set("Time", Application::GetInstance()->GetTime()*4);
		m_mat->Apply(m_grassDraw);
		Disable(Feature::CullFace);
		m_grid->Draw();
		Enable(Feature::CullFace);
		m_grassDraw->Unbind();*/
	}
}
