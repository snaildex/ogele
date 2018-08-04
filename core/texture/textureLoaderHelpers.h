//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_TEXTURELOADERHELPERS_H
#define OGELE_TEXTURELOADERHELPERS_H
namespace ogele {
    string ReadProperty(const Jzon::Node *reader, const string &propName, const string &defaultValue);

    extern map<const string, TextureWrapMode> StrToTexWrapMode;

    extern map<const string, TextureFilterMode> StrToTexFilterMode;
}
#endif //OGELE_TEXTURELOADERHELPERS_H