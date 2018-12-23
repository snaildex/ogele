#pragma once
namespace ogele {

	class TextureCube::Proxy : public ResourceProxy {
		TextureFormat m_format;
		TextureWrapMode m_wrap;
		TextureFilterMode m_minFilter;
		TextureFilterMode m_magFilter;
		std::vector<std::filesystem::path> m_files;
	public:
		TextureCube::Proxy(
			TextureFormat format,
			TextureWrapMode wrap,
			TextureFilterMode minFilter,
			TextureFilterMode magFilter,
			const std::vector<std::filesystem::path>& files);
		virtual Resource* Build() const override;
	};

	class TextureCubeLoader : public ResourceLoader {
	public:
		ResourceProxy *Load(const Jzon::Node *reader) const override;
	};
}