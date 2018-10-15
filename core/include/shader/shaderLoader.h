#pragma once

#include <map>
#include <string>

#include <jzon/Jzon.h>

#include "../object/resource.h"
#include "../other/enums.h"

namespace ogele {
    class ShaderLoader : public ResourceLoader {
        static std::map<std::string, ShaderType> m_shaderTypeMap;

    public:

        Resource *Load(const Jzon::Node* reader) const override;

    };
}