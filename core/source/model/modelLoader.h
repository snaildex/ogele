#pragma once
namespace ogele {
	class Model::Proxy : public ResourceProxy {
		fs::path m_path;
	public:
		Proxy(const fs::path& path);
		virtual Resource* Build() const override;
	};

	class ModelLoader : public ResourceLoader {
	public:
		bool CanLoad(const fs::path& file) const override;
		std::vector<ResourceProxy*> Load() const override;
	};
}