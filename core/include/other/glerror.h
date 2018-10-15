#pragma once

#include <stdexcept>

#include <GL/glew.h>

namespace ogele {
    class GLError : public std::runtime_error {
        GLenum m_code;
    public:
        GLError(GLenum code) noexcept;

        const char *what() const noexcept override;

        const wchar_t *whatW() const;

    };

    void CheckGLError();

#ifdef _DEBUG
#define GLErr() CheckGLError()
#else
#define GLErr()
#endif //_DEBUG
}
