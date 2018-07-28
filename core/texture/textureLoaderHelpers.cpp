//
// Created by ??????? on 21.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    string ReadProperty(const Jzon::Node *reader, const string &propName, const string &defaultValue) {
        if(reader->has(propName))
            return reader->get(propName).toString();
        else return defaultValue;
    }

    map<const string, TextureWrapMode> StrToTexWrapMode = {
            {"clampToEdge",       TextureWrapMode::ClampToEdge},
            {"clampToBorder",     TextureWrapMode::ClampToBorder},
            {"mirroredRepeat",    TextureWrapMode::MirroredRepeat},
            {"repeat",            TextureWrapMode::Repeat},
            {"mirrorClampToEdge", TextureWrapMode::MirrorClampToEdge}
    };
    map<const string, TextureFilterMode> StrToTexFilterMode = {
            {"linear",               TextureFilterMode::Linear},
            {"linearMipMapLinear",   TextureFilterMode::LinearMipMapLinear},
            {"linearMipMapNearest",  TextureFilterMode::LinearMipMapNearest},
            {"nearest",              TextureFilterMode::Nearest},
            {"nearestMipMapLinear",  TextureFilterMode::NearestMipMapLinear},
            {"nearestMipMapNearest", TextureFilterMode::NearestMipMapNearest}
    };
}

