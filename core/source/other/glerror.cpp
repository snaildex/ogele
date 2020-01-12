#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {
	GLError::GLError(GLErrorType type, const std::string& file, int line) noexcept :
		std::runtime_error("OpenGL runtime error") {
		stringstream str;
		str << '[' << file << ':' << line << "] ";
		switch (type) {
		case GLErrorType::InvalidOperation:
			str << "Invalid operation";
			break;
		case GLErrorType::InvalidEnum:
			str << "Invalid enum";
			break;
		case GLErrorType::InvalidValue:
			str << "Invalid value";
			break;
		case GLErrorType::OutOfMemory:
			str << "Out of memory";
			break;
		case GLErrorType::InvalidFramebufferOperation:
			str << "Invalid frame buffer operation";
			break;
		case GLErrorType::StackUnderflow:
			str << "Stack underflow";
			break;
		case GLErrorType::StackOverflow:
			str << "Stack overflow";
			break;
		default:
			str << "Code " + (int)type;
			break;
		}
		m_text = str.str();
	}

	const char *GLError::what() const noexcept {
		return m_text.c_str();
	}

	void CheckGLError(const std::string& file, int line) {
		GLErrorType err = (GLErrorType)glGetError();
		if (err == GLErrorType::NoError) return;
		throw GLError(err, file, line);
	}

}