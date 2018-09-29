//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_SHADERSTAGE_H
#define OGELE_SHADERSTAGE_H
namespace ogele
{
    class ShaderStage : public HandleBase<GLenum>
    {
    public:
        ShaderStage(ShaderType type, const std::string& source);
        ~ShaderStage();
    };
}
#endif //OGELE_SHADERSTAGE_H
