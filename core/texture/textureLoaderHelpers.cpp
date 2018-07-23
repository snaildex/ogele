//
// Created by ??????? on 21.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    string ReadXMLProperty(const tinyxml2::XMLElement *reader, const string &propName, const string &defaultValue) {
        auto elem = reader->FirstChildElement(propName.data());
        if (elem == nullptr)
            return defaultValue;
        else
            return elem->GetText();
    }

    map<const string, TextureWrapMode> StrToTexWrapMode = {
            {"ClampToEdge",       TextureWrapMode::ClampToEdge},
            {"ClampToBorder",     TextureWrapMode::ClampToBorder},
            {"MirroredRepeat",    TextureWrapMode::MirroredRepeat},
            {"Repeat",            TextureWrapMode::Repeat},
            {"MirrorClampToEdge", TextureWrapMode::MirrorClampToEdge}
    };
    map<const string, TextureFilterMode> StrToTexFilterMode = {
            {"Linear",               TextureFilterMode::Linear},
            {"LinearMipMapLinear",   TextureFilterMode::LinearMipMapLinear},
            {"LinearMipMapNearest",  TextureFilterMode::LinearMipMapNearest},
            {"Nearest",              TextureFilterMode::Nearest},
            {"NearestMipMapLinear",  TextureFilterMode::NearestMipMapLinear},
            {"NearestMipMapNearest", TextureFilterMode::NearestMipMapNearest}
    };
}

