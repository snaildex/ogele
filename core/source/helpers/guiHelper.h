#pragma once
namespace ogele {

    template<typename T>
    void PropertyGUI(const std::string &name, T *value);

/*
    template<typename T>
    void PropertyGUI(const string &name, T *value) {
        ImGui::TextDisabled(name.c_str());
    }
*/

}
