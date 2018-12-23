#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {
	ResourceContainer::ResourceContainer() {
		m_loaders["Shader"] = make_unique<ShaderLoader>();
		m_loaders["Texture2D"] = make_unique<Texture2DLoader>();
		m_loaders["TextureCube"] = make_unique<TextureCubeLoader>();
		m_loaders["Texture2DArray"] = make_unique<Texture2DArrayLoader>();
		m_loaders["RenderPipeline"] = make_unique<RenderPipelineLoader>();
		m_loaders["Model"] = make_unique<ModelLoader>();
	}

	void ResourceContainer::LoadFromFilesystem() {
		fs::path cpath = fs::current_path();
		Jzon::Parser parser;
		list<fs::path> configs = ScanFiles(".json");
		for (const auto &p : configs) {
			Jzon::Node node = parser.parseFile(p.string());
			if (node.isValid()) {
				fs::current_path(p.parent_path());
				if (node.isArray())
					for (int i = 0; i < node.getCount(); i++) {
						Jzon::Node cnode = node.get(i);
						AddResource(m_loaders[cnode.get("type").toString()]->Load(&cnode));
					}
				else
					AddResource(m_loaders[node.get("type").toString()]->Load(&node));
			}
		}
		fs::current_path(cpath);
	}

	void ResourceContainer::GUI() {

	}

}