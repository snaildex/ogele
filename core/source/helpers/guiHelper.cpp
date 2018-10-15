#include <helpers/guiHelper.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui/imgui.h>

using namespace std;
using namespace glm;

namespace ogele {

    template<>
    void PropertyGUI(const string &name, float *value) { ImGui::InputFloat(name.c_str(), value); }

    template<>
    void PropertyGUI(const string &name, vec2 *value) { ImGui::InputFloat2(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, vec3 *value) { ImGui::InputFloat3(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, vec4 *value) { ImGui::InputFloat4(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, int *value) { ImGui::InputInt(name.c_str(), value); }

    template<>
    void PropertyGUI(const string &name, ivec2 *value) { ImGui::InputInt2(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, ivec3 *value) { ImGui::InputInt3(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, ivec4 *value) { ImGui::InputInt4(name.c_str(), &value->x); }

    template<>
    void PropertyGUI(const string &name, unsigned int *value) {
        ImGui::LabelText(name.c_str(), to_string(*value).c_str());
    }

    template<>
    void PropertyGUI(const string &name, uvec2 *value) {
        ImGui::LabelText(name.c_str(), to_string(*value).c_str());
    }

    template<>
    void PropertyGUI(const string &name, uvec3 *value) {
        ImGui::LabelText(name.c_str(), to_string(*value).c_str());
    }

    template<>
    void PropertyGUI(const string &name, uvec4 *value) {
        ImGui::LabelText(name.c_str(), to_string(*value).c_str());
    }

#define NoGUI(type) template<> void PropertyGUI(const string &name, type *value) { ImGui::LabelText(name.c_str(), typeid(type).name()); }

    NoGUI(double)
    NoGUI(dvec2)
    NoGUI(dvec3)
    NoGUI(dvec4)
    NoGUI(mat2)
    NoGUI(mat2x3)
    NoGUI(mat2x4)
    NoGUI(mat3x2)
    NoGUI(mat3)
    NoGUI(mat3x4)
    NoGUI(mat4x2)
    NoGUI(mat4x3)
    NoGUI(mat4)
    NoGUI(dmat2)
    NoGUI(dmat2x3)
    NoGUI(dmat2x4)
    NoGUI(dmat3x2)
    NoGUI(dmat3)
    NoGUI(dmat3x4)
    NoGUI(dmat4x2)
    NoGUI(dmat4x3)
    NoGUI(dmat4)

}
