#pragma once
namespace ogele {
	enum class GLErrorType {
		NoError = GL_NO_ERROR,
		InvalidEnum = GL_INVALID_ENUM,
		InvalidValue = GL_INVALID_VALUE,
		InvalidOperation = GL_INVALID_OPERATION,
		InvalidFramebufferOperation = GL_INVALID_FRAMEBUFFER_OPERATION,
		OutOfMemory = GL_OUT_OF_MEMORY,
		StackUnderflow = GL_STACK_UNDERFLOW,
		StackOverflow = GL_STACK_OVERFLOW
	};

	class GLError : public std::runtime_error {
		std::string m_text;
	public:
		GLError(GLErrorType type, const std::string& file, int line) noexcept;

		const char *what() const noexcept override;

	};

	void CheckGLError(const std::string& file, int line);

#ifdef _DEBUG
#define GLErr() CheckGLError(__FILE__,__LINE__)
#else
#define GLErr()
#endif //_DEBUG
}
