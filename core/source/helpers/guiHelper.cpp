#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
    template<>
    void PropertyGUI(const string &name, float *value) {  }

    template<>
    void PropertyGUI(const string &name, vec2 *value) {  }

    template<>
    void PropertyGUI(const string &name, vec3 *value) {  }

    template<>
    void PropertyGUI(const string &name, vec4 *value) {  }

    template<>
    void PropertyGUI(const string &name, int *value) {  }

    template<>
    void PropertyGUI(const string &name, ivec2 *value) {  }

    template<>
    void PropertyGUI(const string &name, ivec3 *value) {  }

    template<>
    void PropertyGUI(const string &name, ivec4 *value) {  }

    template<>
    void PropertyGUI(const string &name, unsigned int *value) {
        
    }

    template<>
    void PropertyGUI(const string &name, uvec2 *value) {
        
    }

    template<>
    void PropertyGUI(const string &name, uvec3 *value) {
        
    }

    template<>
    void PropertyGUI(const string &name, uvec4 *value) {
        
    }

#define NoGUI(type) template<> void PropertyGUI(const string &name, type *value) {  }

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
