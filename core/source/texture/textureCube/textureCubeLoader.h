//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_TEXTURECUBELOADER_H
#define OGELE_TEXTURECUBELOADER_H
namespace ogele {
    class TextureCubeLoader : public ResourceLoader {

    public:

        Resource *Load(const Jzon::Node *reader) const override;

    };
}
#endif //OGELE_TEXTURE2DLOADER_H
