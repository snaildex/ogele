#pragma once
namespace ogele {

	class TextureCube::Proxy : public ResourceProxy {
		TextureFormat m_format;
		TextureWrapMode m_wrap;
		TextureFilterMode m_minFilter;
		TextureFilterMode m_magFilter;
		std::array<std::filesystem::path, 6> m_files;
	public:
		TextureCube::Proxy(
			TextureFormat format,
			TextureWrapMode wrap,
			TextureFilterMode minFilter,
			TextureFilterMode magFilter,
			const std::array<std::filesystem::path, 6>& files);
		virtual Resource* Build() const override;
	};

	class TextureCubeLoader : public ResourceLoader {
	public:
		bool CanLoad(const fs::path& file) const override;
		std::vector<ResourceProxy*> Load() const override;
	};
}