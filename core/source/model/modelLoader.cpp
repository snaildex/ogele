#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Model::Proxy::Proxy(const fs::path& path, const std::vector<std::string>& shader) {
		m_path = path;
		m_shader = shader;
	}

	void ProcessNode(Model* model, Transform* tr, aiNode* node) {
		tr->CreateActor(node->mName.C_Str());
		vec3 pos;
		aiQuaternion rot;
		node->mTransformation.DecomposeNoScaling(rot, reinterpret_cast<aiVector3D&>(pos));
		quat nrot;
		nrot.x = rot.x;
		nrot.y = rot.y;
		nrot.z = rot.z;
		nrot.w = rot.w;
		tr->SetLocalPos(pos);
		tr->SetLocalRot(nrot);
		if (node->mNumMeshes > 0) {
			Model::Node* mnode = tr->GetActor()->AddComponent<Model::Node>();
			for (size_t i = 0; i < node->mNumMeshes; ++i)
				mnode->AddMesh(model->GetMesh(node->mMeshes[i]));
		}
		for (int i = 0; i < node->mNumChildren; ++i)
			ProcessNode(model, tr->CreateChild(), node->mChildren[i]);
	}

	Resource* Model::Proxy::Build() const {
		Assimp::Importer import;
		import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		const aiScene *scene = import.ReadFile(m_path.string(), aiProcessPreset_TargetRealtime_MaxQuality);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			throw exception(import.GetErrorString());
		Model* res = new Model(scene->mNumMaterials, m_shader);
		res->CopyNameTagsFrom(this);
		if (scene->HasMeshes())
			for (int i = 0; i < scene->mNumMeshes; ++i) {
				aiMesh *m = scene->mMeshes[i];
				unsigned int vertCount = m->mNumVertices;
				ModelMesh* mesh = new ModelMesh();
				if (m->HasPositions()) {
					FloatBuffer<vec3>* buf = new FloatBuffer<vec3>(BufferFlags::None, vertCount, reinterpret_cast<vec3*>(m->mVertices));
					mesh->SetPositionBuffer(buf);
				}
				if (m->HasNormals()) {
					FloatBuffer<vec3>* buf = new FloatBuffer<vec3>(BufferFlags::None, vertCount, reinterpret_cast<vec3*>(m->mNormals));
					mesh->SetNormalsBuffer(buf);
				}
				if (m->HasTangentsAndBitangents()) {
					FloatBuffer<vec3>* buf = new FloatBuffer<vec3>(BufferFlags::None, vertCount, reinterpret_cast<vec3*>(m->mTangents));
					mesh->SetTangentsBuffer(buf);
					buf = new FloatBuffer<vec3>(BufferFlags::None, vertCount, reinterpret_cast<vec3*>(m->mBitangents));
					mesh->SetBitangentsBuffer(buf);
				}
				for (int j = 0; j < 4; ++j)
					if (m->HasVertexColors(j)) {
						FloatBuffer<vec4>* buf = new FloatBuffer<vec4>(BufferFlags::None, vertCount, reinterpret_cast<vec4*>(m->mColors[j]));
						mesh->SetColorBuffer(j, buf);
					}
				for (int j = 0; j < 4; ++j)
					if (m->HasTextureCoords(j)) {
						FloatBuffer<vec3>* buf = new FloatBuffer<vec3>(BufferFlags::None, vertCount, reinterpret_cast<vec3*>(m->mTextureCoords[j]));
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
		return res;
	}

	ResourceProxy *ModelLoader::Load(const Jzon::Node *reader) const {
		Model::Proxy* res = new Model::Proxy(
			fs::absolute(reader->get("file").toString()),
			ReadStringArrayProperty(reader, "shader"));
		LoadNameTags(reader, res);
		return res;
	}
}