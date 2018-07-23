//
// Created by ??????? on 21.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    ShaderStage::ShaderStage(ShaderType type, const std::string &source) {
        m_handle = glCreateShader(static_cast<GLenum>(type));
        GLErr();
        auto p = source.data();
        glShaderSource(m_handle, 1, &p, nullptr);
        GLErr();
        glCompileShader(m_handle);
        GLErr();

        GLint Result = GL_FALSE;
        int InfoLogLength;
        glGetShaderiv(m_handle, GL_COMPILE_STATUS, &Result);
        GLErr();
        if (Result == GL_FALSE) {
            glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &InfoLogLength);
            GLErr();
            char *message = new char[InfoLogLength + 1];
            glGetShaderInfoLog(m_handle, InfoLogLength, nullptr, &message[0]);
            GLErr();
            throw ShaderCompileException(message);
        }
    }

    ShaderStage::~ShaderStage() {
        glDeleteShader(m_handle);
        GLErr();
    }
}
