#pragma once
namespace ogele
{
    class ShaderCompileException : public std::runtime_error
    {
		std::string m_name;
        std::string m_message;
    public:
        ShaderCompileException(const std::string& name, const std::string& message);

        const char* what() const noexcept override;

        ~ShaderCompileException();
    };

}