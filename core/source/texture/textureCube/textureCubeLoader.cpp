//
// Created by ??????? on 21.07.2018.
//

#include <other/stb_image.h>
#include <ogele.h>

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

const array<string, 6> facesNames = {
        "right",
        "left",
        "up",
        "down",
        "back",
        "front"
};

namespace ogele {
    Resource *TextureCubeLoader::Load(const Jzon::Node *reader) const {
        auto filePath = reader->get("file");
        int x, y, n;
        ucvec4 *data = reinterpret_cast<ucvec4 *>(
                stbi_load(filePath.get(facesNames[0]).toString().c_str(), &x, &y, &n, 4));
        ivec2 size = {x, y};
        TextureCube *tex = new TextureCube(size, true, TextureFormat::RGBA8);
        LoadNameTags(reader, tex);
        tex->Bind();
        tex->bSetWrap(StrToTexWrapMode[ReadProperty(reader, "wrap", "clampToEdge")]);
        tex->bSetMinFilter(StrToTexFilterMode[ReadProperty(reader, "minFilter", "linearMipMapNearest")]);
        tex->bSetMagFilter(StrToTexFilterMode[ReadProperty(reader, "magFilter", "linear")]);
        tex->bSetData(0, 0, {0, 0}, size, data);
        stbi_image_free(data);
        for (int i = 1; i < 6; i++) {
            data = reinterpret_cast<ucvec4 *>(stbi_load(filePath.get(facesNames[i]).toString().c_str(), &x, &y, &n, 4));
            tex->bSetData(i, 0, {0, 0}, size, data);
            stbi_image_free(data);
        }
        tex->Unbind();
        return tex;
    }
}
