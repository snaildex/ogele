//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_SHADERCOMPILEEXCEPTION_H
#define OGELE_SHADERCOMPILEEXCEPTION_H
namespace ogele
{
    class ShaderCompileException : public std::runtime_error
    {
        std::string m_message;
    public:
        ShaderCompileException(const std::string& message);

        const char* what() const noexcept override;

        ~ShaderCompileException();
    };

}
#endif //OGELE_SHADERCOMPILEEXCEPTION_H
