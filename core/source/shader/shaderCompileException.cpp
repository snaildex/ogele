#include <stdafx.h>
#include <ogele.h>
using namespace std;
namespace ogele {
	ShaderCompileException::ShaderCompileException(const std::string& name, const std::string &message) :
		std::runtime_error("Shader compile exception") {
		m_name = name;
		m_message = message;
	}

	const char *ShaderCompileException::what() const noexcept {
		stringstream str;
		str << m_name << " compile error:" << endl << m_message;
		return str.str().data();
	}

	ShaderCompileException::~ShaderCompileException() {
	}
}