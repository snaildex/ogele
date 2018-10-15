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
        auto filePath = reader->get("file").toString();
        int x, y, n;
        ucvec4 *data = reinterpret_cast<ucvec4 *>(stbi_load(filePath.c_str(), &x, &y, &n, 4));
        ivec2 size = {x, y};
        Texture2DArray *tex = new Texture2DArray(size, true, TextureFormat::RGBA8);
        LoadNameTags(reader, tex);
        tex->Bind();
        tex->bSetWrap(StrToTexWrapMode[ReadProperty(reader, "wrap", "repeat")]);
        tex->bSetMinFilter(StrToTexFilterMode[ReadProperty(reader, "minFilter", "linearMipMapNearest")]);
        tex->bSetMagFilter(StrToTexFilterMode[ReadProperty(reader, "magFilter", "linear")]);
        tex->bSetData(0, {0, 0}, size, data);
        tex->Unbind();
        stbi_image_free(data);
        return tex;
    }
}
