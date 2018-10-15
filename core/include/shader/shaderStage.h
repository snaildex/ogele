#pragma once

#include <string>

#include "../helpers/helpers.h"
#include "../other/enums.h"

namespace ogele
{
    class ShaderStage : public HandleBase<GLenum>
    {
    public:
        ShaderStage(ShaderType type, const std::string& source);
        ~ShaderStage();
    };
}