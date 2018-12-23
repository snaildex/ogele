#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Texture2D::Proxy::Proxy(
		TextureFormat format,
		TextureWrapMode wrap,
		TextureFilterMode minFilter,
		TextureFilterMode magFilter,
		const std::filesystem::path& file) {
		m_format = format;
		m_wrap = wrap;
		m_minFilter = minFilter;
		m_magFilter = magFilter;
		m_file = file;
	}
	ResourceProxy *Texture2DLoader::Load(const Jzon::Node *reader) const {
		Texture2D::Proxy* res = new Texture2D::Proxy(
			StrToTexFormat.at(ReadStringProperty(reader, "format", "RGBA8")),
			StrToTexWrapMode.at(ReadStringProperty(reader, "wrap", "repeat")),
			StrToTexFilterMode.at(ReadStringProperty(reader, "minFilter", "linearMipMapNearest")),
			StrToTexFilterMode.at(ReadStringProperty(reader, "magFilter", "linear")),
			fs::absolute(reader->get("file").toString())
		);
		LoadNameTags(reader, res);
		return res;
	}
	Resource * Texture2D::Proxy::Build() const {
		int x, y;
		ucvec4 *data = reinterpret_cast<ucvec4 *>(stbi_load(m_file.string().c_str(), &x, &y, nullptr, 4));
		ivec2 size = { x, y };
		Texture2D *tex = new Texture2D(size, true, m_format);
		tex->CopyNameTagsFrom(this);
		tex->Bind();
		tex->bSetWrap(m_wrap);
		tex->bSetMinFilter(m_minFilter);
		tex->bSetMagFilter(m_magFilter);
		tex->bSetData(0, { 0, 0 }, size, data);
		tex->Unbind();
		stbi_image_free(data);
		return tex;
	}
}
