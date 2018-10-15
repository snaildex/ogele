#include <texture/texture2D/texture2D.h>
#include <texture/texture2D/texture2DLoader.h>
#include <texture/textureLoaderHelpers.h>
#include <glm/glm.hpp>
#include <buffer/vectors.h>

#define STB_IMAGE_IMPLEMENTATION
#include <other/stb_image.h>

using namespace std;
using namespace glm;

namespace ogele {
    Resource *Texture2DLoader::Load(const Jzon::Node *reader) const {
        auto filePath = reader->get("file").toString();
        int x, y, n;
        ucvec4 *data = reinterpret_cast<ucvec4 *>(stbi_load(filePath.c_str(), &x, &y, &n, 4));
        ivec2 size = {x, y};
        Texture2D *tex = new Texture2D(size, true, TextureFormat::RGBA8);
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
