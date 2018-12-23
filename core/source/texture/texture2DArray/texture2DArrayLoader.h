#pragma once
namespace ogele {
	class Texture2DArray::Proxy : public ResourceProxy {
		TextureFormat m_format;
		TextureWrapMode m_wrap;
		TextureFilterMode m_minFilter;
		TextureFilterMode m_magFilter;
		std::vector<std::filesystem::path> m_files;
	public:
		Texture2DArray::Proxy(
			TextureFormat format,
			TextureWrapMode wrap,
			TextureFilterMode minFilter,
			TextureFilterMode magFilter,
			const std::vector<std::filesystem::path>& files);
		virtual Resource* Build() const override;
	};

    class Texture2DArrayLoader : public ResourceLoader {
    public:
        ResourceProxy *Load(const Jzon::Node *reader) const override;
    };
}