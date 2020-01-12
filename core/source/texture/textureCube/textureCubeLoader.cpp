#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {

	const map<string, int> facesNames = {
		{"Right", 0},
		{"Left", 1},
		{"Up", 2},
		{"Down", 3},
		{"Back", 4},
		{"Front", 5}
	};

	TextureCube::Proxy::Proxy(
		TextureFormat format,
		TextureWrapMode wrap,
		TextureFilterMode minFilter,
		TextureFilterMode magFilter,
		const std::array<std::filesystem::path, 6>& files) {
		m_format = format;
		m_wrap = wrap;
		m_minFilter = minFilter;
		m_magFilter = magFilter;
		m_files = files;
	}

	bool TextureCubeLoader::CanLoad(const fs::path& file) const {
		if (!Is2DImage(file)) return false;
		for (const auto& face : facesNames)
			if (CheckTail(file.stem().filename().string(), face.first)) return true;
		return false;
	}

	std::vector<ResourceProxy*> TextureCubeLoader::Load() const {
		std::vector<ResourceProxy*> res;
		map<fs::path, array<fs::path, 6>> texFaces;
		for (const auto& f : GetFiles()) {
			string p = f.stem().string();
			for (const auto& face : facesNames)
				if (CheckTail(p, face.first))
					texFaces[StripTail(p, face.first.length())][face.second] = f;
		}
		for (const auto& tf : texFaces) {
			TextureCube::Proxy *tex = new TextureCube::Proxy(
				TextureFormat::RGBA8,
				TextureWrapMode::Repeat,
				TextureFilterMode::LinearMipMapNearest,
				TextureFilterMode::Linear,
				tf.second
			);
			tex->SetName(tf.first.filename().string());
			res.push_back(tex);
		}
		return res;
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
