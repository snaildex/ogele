#pragma once
namespace ogele {
	class Texture2D::Proxy : public ResourceProxy {
		TextureFormat m_format;
		TextureWrapMode m_wrap;
		TextureFilterMode m_minFilter;
		TextureFilterMode m_magFilter;
		std::filesystem::path m_file;
	public:
		Texture2D::Proxy(
			TextureFormat format,
			TextureWrapMode wrap,
			TextureFilterMode minFilter,
			TextureFilterMode magFilter,
			const std::filesystem::path& file);
		virtual Resource* Build() const override;
	};

	class Texture2DLoader : public ResourceLoader {
	public:
		ResourceProxy *Load(const Jzon::Node *reader) const override;
	};
}