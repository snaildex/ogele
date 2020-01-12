#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {
	ResourceContainer::ResourceContainer() {
		m_loaders.emplace_back(new ShaderLoader());
		m_loaders.emplace_back(new ModelLoader());
		m_loaders.emplace_back(new RenderPipelineLoader());
		m_loaders.emplace_back(new TextureCubeLoader());
		m_loaders.emplace_back(new Texture2DArrayLoader());
		m_loaders.emplace_back(new Texture2DLoader());
	}

	void ResourceContainer::HandleDirectory(const fs::path& path) {
		Log("%s/", path.filename().string().c_str());
		LogIndent;
		for (auto& p : fs::directory_iterator(path)) {
			if (p.is_directory()) {
				HandleDirectory(p);
			}
			if (p.is_regular_file())
				for (auto& loader : m_loaders)
					if (loader->CanLoad(fs::absolute(p.path()))) {
						Log("+ %s", p.path().filename().string().data());
						loader->EnlistFile(fs::absolute(p.path()));
						break;
					}
		}
	}

	void ResourceContainer::LoadFromFilesystem() {
		fs::path cpath = fs::current_path();
		HandleDirectory(cpath);
		fs::current_path(cpath);
		for (auto& loader : m_loaders) {
			vector<ResourceProxy*> res;
			LogSpace();
			Log("Loading from %s", typeid(*loader.get()).name());
			{
				LogIndent;
				res = loader->Load();
			}
			Log("Loaded resources:");
			{
				LogIndent;
				for (auto& r : res) {
					AddResource(r);
					Log("%s %s",r->GetName().data(), r->PrintTags().data());
				}
			}
		}
	}

	void ResourceContainer::GUI() {

	}

}