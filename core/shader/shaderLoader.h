//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_SHADERLOADER_H
#define OGELE_SHADERLOADER_H
namespace ogele {
    class ShaderLoader : public ResourceLoader {
        static map<string, ShaderType> m_shaderTypeMap;

    public:

        Resource *Load(const tinyxml2::XMLElement *reader) const override;

    };
}
#endif //OGELE_SHADERLOADER_H
