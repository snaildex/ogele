//
// Created by ??????? on 22.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    void Material::AddUniform(const string &name, UniformType type) {
        switch (type) {
#define MaterialTypeCase(EnumName, TypeName) case UniformType:: EnumName : m_data[name] = unique_ptr<UniformBase>(new UniformValue< TypeName >()); break
            MaterialTypeCase(Float, float);
            MaterialTypeCase(FloatVec2, vec2);
            MaterialTypeCase(FloatVec3, vec3);
            MaterialTypeCase(FloatVec4, vec4);

            MaterialTypeCase(Double, double);
            MaterialTypeCase(DoubleVec2, dvec2);
            MaterialTypeCase(DoubleVec3, dvec3);
            MaterialTypeCase(DoubleVec4, dvec4);

            MaterialTypeCase(Int, int);
            MaterialTypeCase(IntVec2, ivec2);
            MaterialTypeCase(IntVec3, ivec3);
            MaterialTypeCase(IntVec4, ivec4);

            MaterialTypeCase(UnsignedInt, unsigned int);
            MaterialTypeCase(UnsignedIntVec2, uvec2);
            MaterialTypeCase(UnsignedIntVec3, uvec3);
            MaterialTypeCase(UnsignedIntVec4, uvec4);

            MaterialTypeCase(FloatMat2, mat2);
            MaterialTypeCase(FloatMat2x3, mat2x3);
            MaterialTypeCase(FloatMat2x4, mat2x4);
            MaterialTypeCase(FloatMat3x2, mat3x2);
            MaterialTypeCase(FloatMat3, mat3);
            MaterialTypeCase(FloatMat3x4, mat3x4);
            MaterialTypeCase(FloatMat4x2, mat4x2);
            MaterialTypeCase(FloatMat4x3, mat4x3);
            MaterialTypeCase(FloatMat4, mat4);

            MaterialTypeCase(DoubleMat2, dmat2);
            MaterialTypeCase(DoubleMat2x3, dmat2x3);
            MaterialTypeCase(DoubleMat2x4, dmat2x4);
            MaterialTypeCase(DoubleMat3x2, dmat3x2);
            MaterialTypeCase(DoubleMat3, dmat3);
            MaterialTypeCase(DoubleMat3x4, dmat3x4);
            MaterialTypeCase(DoubleMat4x2, dmat4x2);
            MaterialTypeCase(DoubleMat4x3, dmat4x3);
            MaterialTypeCase(DoubleMat4, dmat4);
        }
    }

    void Material::GUI() {
        for (const auto &u : m_data)
            u.second->GUI(u.first);
    }

    void Material::Apply(ShaderProgram *sh) {
        for (const auto &u : m_data)
            u.second->Apply(u.first, sh);
    }

    void Material::FetchValues(Material *mat) {
        for (const auto &mu : mat->m_data)
            for (const auto &u : m_data)
                if (u.first == mu.first)
                    u.second->TryCopyFrom(mu.second.get());
    }
}
