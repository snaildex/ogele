#pragma once
namespace ogele {
    class GLError : public std::runtime_error {
		std::string m_text;
    public:
        GLError(GLenum code, const std::string& file, int line) noexcept;

        const char *what() const noexcept override;

    };

    void CheckGLError(const std::string& file, int line);

#ifdef _DEBUG
#define GLErr() CheckGLError(__FILE__,__LINE__)
#else
#define GLErr()
#endif //_DEBUG
}
