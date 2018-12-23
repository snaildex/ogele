#pragma once
namespace ogele
{
    class ShaderStage : public HandleBase<GLenum>
    {
    public:
        ShaderStage(const std::string& name, ShaderType type, const std::string& source);
        ~ShaderStage();
    };
}