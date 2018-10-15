#include <other/glerror.h>

using namespace std;

namespace ogele {
    GLError::GLError(GLenum code) noexcept : std::runtime_error("OpenGL runtime error") {
        m_code = code;
    }

    const char *GLError::what() const noexcept {
        switch (m_code) {
            case GL_INVALID_OPERATION:
                return "Invalid operation";
            case GL_INVALID_ENUM:
                return "Invalid enum";
            case GL_INVALID_VALUE:
                return "Invalid value";
            case GL_OUT_OF_MEMORY:
                return "Out of memory";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return "Invalid frame buffer operation";
            case GL_STACK_UNDERFLOW:
                return "Stack underflow";
            case GL_STACK_OVERFLOW:
                return "Stack overflow";
            default:
                return "Code " + m_code;
        }
    }

    const wchar_t *GLError::whatW() const {
        switch (m_code) {
            case GL_INVALID_OPERATION:
                return L"Invalid operation";
            case GL_INVALID_ENUM:
                return L"Invalid enum";
            case GL_INVALID_VALUE:
                return L"Invalid value";
            case GL_OUT_OF_MEMORY:
                return L"Out of memory";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return L"Invalid frame buffer operation";
            case GL_STACK_UNDERFLOW:
                return L"Stack underflow";
            case GL_STACK_OVERFLOW:
                return L"Stack overflow";
            default:
                return L"Code " + m_code;
        }
    }

    void CheckGLError() {
        auto err = glGetError();
        if (err == GL_NO_ERROR) return;
        throw GLError(err);
    }

}