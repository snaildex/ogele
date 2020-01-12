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

	bool Texture2DArrayLoader::CanLoad(const fs::path& file) const {
		if (!Is2DImage(file)) return false;
		return StripTailNum(file.stem().filename().string()) >= 0;
	}

	std::vector<ResourceProxy*> Texture2DArrayLoader::Load() const {
		std::vector<ResourceProxy*> res;
		map<fs::path, vector<fs::path>> texSlices;
		for (const auto& f : GetFiles()) {
			string p = (f.parent_path() / f.stem()).string();
			string cp;
			int num = StripTailNum(p, cp);
			if (texSlices[cp].size() <= num) texSlices[cp].resize(num + 1);
			texSlices[cp][num] = f;
		}
		for (const auto& tf : texSlices) {
			Texture2DArray::Proxy *tex = new Texture2DArray::Proxy(
				TextureFormat::RGBA8,
				TextureWrapMode::Repeat,
				TextureFilterMode::LinearMipMapNearest,
				TextureFilterMode::Linear,
				tf.second
			);
			LoadNameTags(tex, tf.first);
			Log(tex->GetName().c_str());
			LogIndent;
			for (int i = 0; i < tf.second.size(); ++i) Log("%d: %s", i, Application::RelativePath(tf.second[i]).string().c_str());
			res.push_back(tex);
		}
		return res;
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
