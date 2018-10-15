#pragma once

#include <map>
#include <string>

#include <jzon/Jzon.h>

#include "../other/enums.h"

namespace ogele {
    std::string ReadProperty(const Jzon::Node *reader, const std::string &propName, const std::string &defaultValue);

    extern std::map<const std::string, TextureWrapMode> StrToTexWrapMode;

    extern std::map<const std::string, TextureFilterMode> StrToTexFilterMode;
}