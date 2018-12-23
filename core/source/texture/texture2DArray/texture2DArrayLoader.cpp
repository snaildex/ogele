#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Texture2DArray::Proxy::Proxy(
		TextureFormat format,
		TextureWrapMode wrap,
		TextureFilterMode minFilter,
		TextureFilterMode magFilter,
		const std::vector<std::filesystem::path>& files) {
		m_format = format;
		m_wrap = wrap;
		m_minFilter = minFilter;
		m_magFilter = magFilter;
		m_files = files;
	}

	ResourceProxy* Texture2DArrayLoader::Load(const Jzon::Node *reader) const {
		int count = reader->get("file").getCount();
		std::vector<std::filesystem::path> files;
		for (int i = 0; i < count; i++)
			files.emplace_back(fs::absolute(reader->get("file").get(i).toString()));
		Texture2DArray::Proxy *tex = new Texture2DArray::Proxy(
			StrToTexFormat.at(ReadStringProperty(reader, "format", "RGBA8")),
			StrToTexWrapMode.at(ReadStringProperty(reader, "wrap", "repeat")),
			StrToTexFilterMode.at(ReadStringProperty(reader, "minFilter", "linearMipMapNearest")),
			StrToTexFilterMode.at(ReadStringProperty(reader, "magFilter", "linear")),
			files
		);
		LoadNameTags(reader, tex);
		return tex;
	}

	Resource* Texture2DArray::Proxy::Build() const {
		int x, y, n;
		ucvec4 **data;
		int count = m_files.size();
		data = new ucvec4*[count];
		for (size_t i = 0; i < count; i++)
			data[i] = reinterpret_cast<ucvec4 *>(stbi_load(m_files[i].string().c_str(), &x, &y, &n, 4));
		Texture2DArray *tex = new Texture2DArray({ x, y, count }, true, m_format);
		tex->CopyNameTagsFrom(this);
		tex->Bind();
		tex->bSetWrap(m_wrap);
		tex->bSetMinFilter(m_minFilter);
		tex->bSetMagFilter(m_magFilter);
		for (int i = 0; i < count; i++)
			tex->bSetData(0, { 0, 0, i }, { x, y, 1 }, data[i]);
		tex->Unbind();
		for (int i = 0; i < count; i++)
			stbi_image_free(data[i]);
		return tex;
	}
}
