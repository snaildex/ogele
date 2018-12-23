#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {
    GLError::GLError(GLenum code, const std::string& file, int line) noexcept : 
		std::runtime_error("OpenGL runtime error") {
        stringstream str;
		str << '[' << file << ':' << line << "] ";
		switch (code) {
		case GL_INVALID_OPERATION:
			str << "Invalid operation";
			break;
		case GL_INVALID_ENUM:
			str << "Invalid enum";
			break;
		case GL_INVALID_VALUE:
			str << "Invalid value";
			break;
		case GL_OUT_OF_MEMORY:
			str << "Out of memory";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			str << "Invalid frame buffer operation";
			break;
		case GL_STACK_UNDERFLOW:
			str << "Stack underflow";
			break;
		case GL_STACK_OVERFLOW:
			str << "Stack overflow";
			break;
		default:
			str << "Code " + code;
			break;
		}
		m_text = str.str();
    }

    const char *GLError::what() const noexcept {
		return m_text.c_str();
    }

    void CheckGLError(const std::string& file, int line) {
        auto err = glGetError();
        if (err == GL_NO_ERROR) return;
        throw GLError(err,file,line);
    }

}