#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {

	RenderPipelineProxy::RenderPipelineProxy(const fs::path & sourcePath)
	{
		m_sourcePath = sourcePath;
	}

	Resource* RenderPipelineProxy::Build() const {
		map<string, RenderTarget*> targets;
		RenderPipeline *res = new RenderPipeline(m_sourcePath);
		res->CopyNameTagsFrom(this);
		return res;
	}

	vector<ResourceProxy*> RenderPipelineLoader::Load() const {
		Jzon::Parser parser;
		std::vector<ResourceProxy*> res;
		for (const auto& f : GetFiles()) {
			RenderPipelineProxy *proxy = new RenderPipelineProxy(f);
			LoadNameTags(proxy, f.parent_path() / f.stem());
			res.push_back(proxy);
		}
		return res;
	}

	bool RenderPipelineLoader::CanLoad(const fs::path & file) const
	{
		if (file.extension() == ".lua") {
			return CheckTail(file.stem().filename().string(), "Pipeline");
		}
		else
			return false;
	}
}
