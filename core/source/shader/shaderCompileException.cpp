#include <shader/shaderCompileException.h>

using namespace std;

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