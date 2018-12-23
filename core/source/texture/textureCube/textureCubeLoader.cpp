#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	const array<string, 6> facesNames = {
			"right",
			"left",
			"up",
			"down",
			"back",
			"front"
	};

	TextureCube::Proxy::Proxy(
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

	ResourceProxy* TextureCubeLoader::Load(const Jzon::Node *reader) const {
		auto filePath = reader->get("file"); 
		std::vector<std::filesystem::path> files;
		for (int i = 0; i < 6; i++)
			files.emplace_back(fs::absolute(filePath.get(facesNames[i]).toString().c_str()));
		TextureCube::Proxy *tex = new TextureCube::Proxy(
			StrToTexFormat.at(ReadStringProperty(reader, "format", "RGBA8")),
			StrToTexWrapMode.at(ReadStringProperty(reader, "wrap", "repeat")),
			StrToTexFilterMode.at(ReadStringProperty(reader, "minFilter", "linearMipMapNearest")),
			StrToTexFilterMode.at(ReadStringProperty(reader, "magFilter", "linear")),
			files
		);
		LoadNameTags(reader, tex);
		return tex;
	}

	Resource* TextureCube::Proxy::Build() const {
		int x, y;
		ucvec4 **data;
		int count = m_files.size();
		data = new ucvec4*[count];
		for (size_t i = 0; i < count; i++)
			data[i] = reinterpret_cast<ucvec4 *>(stbi_load(m_files[i].string().c_str(), &x, &y, nullptr, 4));
		TextureCube *tex = new TextureCube({ x, y }, true, m_format);
		tex->CopyNameTagsFrom(this);
		tex->Bind();
		tex->bSetWrap(m_wrap);
		tex->bSetMinFilter(m_minFilter);
		tex->bSetMagFilter(m_magFilter);
		for (int i = 0; i < count; i++)
			tex->bSetData(i, 0, { 0, 0 }, { x, y }, data[i]);
		tex->Unbind();
		for (int i = 0; i < count; i++)
			stbi_image_free(data[i]);
		return tex;
	}
}
