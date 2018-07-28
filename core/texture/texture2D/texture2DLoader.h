//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_TEXTURE2DLOADER_H
#define OGELE_TEXTURE2DLOADER_H
namespace ogele {
    class Texture2DLoader : public ResourceLoader {

    public:

        Resource *Load(const Jzon::Node *reader) const override;

    };
}
#endif //OGELE_TEXTURE2DLOADER_H
