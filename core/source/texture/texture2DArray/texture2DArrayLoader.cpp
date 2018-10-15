#include <texture/texture2DArray/texture2DArray.h>
#include <texture/texture2DArray/texture2DArrayLoader.h>
#include <texture/textureLoaderHelpers.h>
#include <glm/glm.hpp>
#include <buffer/vectors.h>
#include <other/stb_image.h>

using namespace std;
using namespace glm;

namespace ogele {
	Resource* Texture2DArrayLoader::Load(const Jzon::Node *reader) const {
		int count = reader->get("file").getCount();
		int x, y, n;
		ucvec4 **data;
		data = new ucvec4*[count];
		for (int i = 0; i < count; i++)
		{
			std::string path = reader->get("file").get(i).toString();
			data[i] = reinterpret_cast<ucvec4 *>(stbi_load(path.c_str(), &x, &y, &n, 4));
		}
		Texture2DArray *tex = new Texture2DArray({ x, y, count }, true, TextureFormat::RGBA8);
		LoadNameTags(reader, tex);
		tex->Bind();
		tex->bSetWrap(StrToTexWrapMode[ReadProperty(reader, "wrap", "repeat")]);
		tex->bSetMinFilter(StrToTexFilterMode[ReadProperty(reader, "minFilter", "linearMipMapNearest")]);
		tex->bSetMagFilter(StrToTexFilterMode[ReadProperty(reader, "magFilter", "linear")]);
		for (int i = 0; i < count; i++)
			tex->bSetData(0, { 0, 0, i }, { x, y, 1 }, data[i]);
		tex->Unbind();
		for (int i = 0; i < count; i++)
			stbi_image_free(data[i]);
		return tex;
	}
}
