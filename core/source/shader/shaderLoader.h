#pragma once
namespace ogele {

	class ShaderProgram::Proxy : public ResourceProxy {
		std::map<ShaderType, std::filesystem::path> m_stages;
	public:
		void AddStage(ShaderType stage, const std::filesystem::path& path) { m_stages[stage] = path; }
		virtual Resource* Build() const override;
	};

	class ShaderLoader : public ResourceLoader {
		static std::map<std::string, ShaderType> m_shaderTypeMap;
	public:
		bool CanLoad(const fs::path& file) const override;
		std::vector<ResourceProxy*> Load() const override;
	};
}