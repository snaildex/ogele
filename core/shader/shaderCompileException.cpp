//
// Created by ??????? on 21.07.2018.
//
#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    ShaderCompileException::ShaderCompileException(const std::string &message) : std::runtime_error(
            "Shader compile exception") {
        m_message = message;
    }

    const char *ShaderCompileException::what() const noexcept {
        return m_message.data();
    }

    ShaderCompileException::~ShaderCompileException() {
    }
}