#pragma once
namespace ogele {
	class RenderPipelineProxy : public ResourceProxy {
		fs::path m_sourcePath;
	public:
		RenderPipelineProxy(const fs::path& sourcePath);
		virtual Resource* Build() const override;
	};

	class RenderPipelineLoader : public ResourceLoader {
	public:
		bool CanLoad(const fs::path& file) const override;
		std::vector<ResourceProxy*> Load() const override;
	};
}