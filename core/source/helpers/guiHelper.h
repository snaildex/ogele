//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_GUIHELPER_H
#define OGELE_GUIHELPER_H
namespace ogele {

    template<typename T>
    void PropertyGUI(const string &name, T *value);

/*
    template<typename T>
    void PropertyGUI(const string &name, T *value) {
        ImGui::TextDisabled(name.c_str());
    }
*/

}
#endif //OGELE_GUIHELPER_H
