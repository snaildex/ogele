#pragma once

#include <stdexcept>
#include <string>

#include <GL/glew.h>

namespace ogele
{
    class ShaderCompileException : public std::runtime_error
    {
        std::string m_message;
    public:
        ShaderCompileException(const std::string& message);

        const char* what() const noexcept override;

        ~ShaderCompileException();
    };

}