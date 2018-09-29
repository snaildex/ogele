//
// Created by ??????? on 18.07.2018.
//

#ifndef OGELE_GLERROR_H
#define OGELE_GLERROR_H
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
#endif //OGELE_GLERROR_H
