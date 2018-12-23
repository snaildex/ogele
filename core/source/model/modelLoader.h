#pragma once
namespace ogele {
	class Model::Proxy : public ResourceProxy {
		fs::path m_path;
		std::vector<std::string> m_shader;
	public:
		Proxy(const fs::path& path, const std::vector<std::string>& shader);
		virtual Resource* Build() const override;
	};

	class ModelLoader : public ResourceLoader {
	public:
		ResourceProxy *Load(const Jzon::Node *reader) const override;
	};
}