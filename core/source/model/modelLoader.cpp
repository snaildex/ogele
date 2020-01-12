#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {

	const set<string> Resolutions = {
			".fbx",
			".dae",
			".gltf",
			".glb",
			".blend",
			".3ds",
			".ase",
			".obj",
			".ifc",
			".xgl",
			".zgl",
			".ply",
			".dxf",
			".lwo",
			".lws",
			".lxo",
			".stl",
			".x",
			".ac",
			".ms3d",
			".cob",
			".scn",
			".bvh"
	};

	bool ModelLoader::CanLoad(const fs::path & file) const
	{
		return Resolutions.count(file.extension().string()) > 0;
	}

	Model::Proxy::Proxy(const fs::path& path) {
		m_path = path;
	}

	void ProcessNode(Model* model, Transform* tr, aiNode* node) {
		tr->CreateActor(node->mName.C_Str());
		vec3 pos, scale;
		aiQuaternion rot;
		node->mTransformation.Decompose(reinterpret_cast<aiVector3D&>(scale), rot, reinterpret_cast<aiVector3D&>(pos));
		tr->SetLocalPos(pos);
		tr->SetLocalRot({ rot.w, rot.x, rot.y, rot.z });
		tr->SetLocalScale(scale);
		if (node->mNumMeshes > 0) {
			tr->GetActor()->AddTag("Model");
			Model::Node* mnode = tr->GetActor()->AddComponent<Model::Node>();
			mnode->FindShaders({ "Model" });
			for (size_t i = 0; i < node->mNumMeshes; ++i)
				mnode->AddMesh(model->GetMesh(node->mMeshes[i]));
		}
		for (int i = 0; i < node->mNumChildren; ++i)
			ProcessNode(model, tr->CreateChild(), node->mChildren[i]);
	}

	Resource* Model::Proxy::Build() const {
		Assimp::Importer import;
		import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		const aiScene *scene = import.ReadFile(m_path.string(), aiProcessPreset_TargetRealtime_Fast);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			throw exception(import.GetErrorString());
		Model* res = new Model(scene->mNumMaterials);
		res->CopyNameTagsFrom(this);
		if (scene->HasMeshes())
			for (int i = 0; i < scene->mNumMeshes; ++i) {
				aiMesh *m = scene->mMeshes[i];
				unsigned int vertCount = m->mNumVertices;
				vector<vec4> buffer(vertCount);
				ModelMesh* mesh = new ModelMesh({ "Model" });
				if (m->HasPositions()) {
					for (int v = 0; v < vertCount; ++v) buffer[v] = vec4(reinterpret_cast<const vec3&>(m->mVertices[v]), 0);
					FloatBuffer<vec4>* buf = new FloatBuffer<vec4>(BufferFlags::None, vertCount, buffer.data());
					mesh->SetPositionBuffer(buf);
				}
				if (m->HasNormals()) {
					for (int v = 0; v < vertCount; ++v) buffer[v] = vec4(reinterpret_cast<const vec3&>(m->mNormals[v]), 0);
					FloatBuffer<vec4>* buf = new FloatBuffer<vec4>(BufferFlags::None, vertCount, buffer.data());
					mesh->SetNormalsBuffer(buf);
				}
				if (m->HasTangentsAndBitangents()) {
					for (int v = 0; v < vertCount; ++v) buffer[v] = vec4(reinterpret_cast<const vec3&>(m->mTangents[v]), 0);
					FloatBuffer<vec4>* buf = new FloatBuffer<vec4>(BufferFlags::None, vertCount, buffer.data());
					mesh->SetTangentsBuffer(buf);
					for (int v = 0; v < vertCount; ++v) buffer[v] = vec4(reinterpret_cast<const vec3&>(m->mBitangents[v]), 0);
					buf = new FloatBuffer<vec4>(BufferFlags::None, vertCount, buffer.data());
					mesh->SetBitangentsBuffer(buf);
				}
				for (int j = 0; j < 4; ++j)
					if (m->HasVertexColors(j)) {
						for (int v = 0; v < vertCount; ++v) buffer[v] = reinterpret_cast<const vec4&>(m->mColors[j][v]);
						FloatBuffer<vec4>* buf = new FloatBuffer<vec4>(BufferFlags::None, vertCount, buffer.data());
						mesh->SetColorBuffer(j, buf);
					}
				for (int j = 0; j < 4; ++j)
					if (m->HasTextureCoords(j)) {
						for (int v = 0; v < vertCount; ++v) buffer[v] = vec4(reinterpret_cast<const vec3&>(m->mTextureCoords[j][v]), 0);
						FloatBuffer<vec4>* buf = new FloatBuffer<vec4>(BufferFlags::None, vertCount, buffer.data());
						mesh->SetUVBuffer(j, buf);
					}
				if (m->HasFaces()) {
					unsigned int elemCount = m->mNumFaces * 3;
					unique_ptr<uint[]> data(new uint[elemCount]);
					for (int j = 0; j < m->mNumFaces; ++j)
						memcpy(&data[j * 3], m->mFaces[j].mIndices, sizeof(uint) * 3);
					UnsignedIntBuffer<uint>* buf = new UnsignedIntBuffer<uint>(BufferFlags::None, elemCount, data.get());
					mesh->SetIndexBuffer(buf);
				}
				mesh->SetName(m->mName.C_Str());
				res->AddMesh(mesh, m->mMaterialIndex);
			}
		ProcessNode(res, res->GetRoot(), scene->mRootNode);
		res->GetRoot()->CreateActor(GetName())->CopyNameTagsFrom(this);
		return res;
	}

	std::vector<ResourceProxy*> ModelLoader::Load() const {
		std::vector<ResourceProxy*> res;
		for (const auto& f : GetFiles()) {
			auto proxy = new Model::Proxy(f);
			LoadNameTags(proxy, f.parent_path() / f.stem());
			res.push_back(proxy);
		}
		return res;
	}
}