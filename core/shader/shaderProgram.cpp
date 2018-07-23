//
// Created by ??????? on 21.07.2018.
//

#include "../ogele.h"

using namespace std;
using namespace glm;
namespace fs = std::experimental::filesystem;

namespace ogele {

    ShaderProgram::ShaderProgram() {
        m_handle = glCreateProgram();
        GLErr();
    }

    void ShaderProgram::Link() {
        glLinkProgram(m_handle);
        GLErr();
        GLint Result = GL_FALSE;
        int InfoLogLength;
        glGetProgramiv(m_handle, GL_LINK_STATUS, &Result);
        GLErr();
        if (Result == GL_FALSE) {
            glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &InfoLogLength);
            GLErr();
            char *message = new char[InfoLogLength + 1];
            glGetProgramInfoLog(m_handle, InfoLogLength, nullptr, &message[0]);
            GLErr();
            throw ShaderCompileException(message);
        }

        GLint numUnifBlocks = 0;
        glGetProgramInterfaceiv(m_handle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numUnifBlocks);
        GLErr();
        const GLenum blockProperties[] = {GL_NAME_LENGTH};
        const GLenum activeUnifProp[] = {GL_ACTIVE_VARIABLES};
        const GLenum unifProperties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};
        GLint numStorBlocks = 0;
        glGetProgramInterfaceiv(m_handle, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &numStorBlocks);
        GLErr();
        Bind();
        for (int blockIx = 0; blockIx < numUnifBlocks; ++blockIx) {
            GLint bufProp[1];
            glGetProgramResourceiv(m_handle, GL_UNIFORM_BLOCK, blockIx, 1, blockProperties, 1, NULL, bufProp);
            GLErr();
            std::vector<char> s(bufProp[0]);
            glGetProgramResourceName(m_handle, GL_UNIFORM_BLOCK, blockIx, bufProp[0], NULL, s.data());
            GLErr();
            glUniformBlockBinding(m_handle, blockIx, blockIx);
            GLErr();
            m_buffers.emplace_back(blockIx, string(s.data()), BufferTarget::Uniform);
        }
        for (int blockIx = 0; blockIx < numStorBlocks; ++blockIx) {
            GLint bufProp[1];
            glGetProgramResourceiv(m_handle, GL_SHADER_STORAGE_BLOCK, blockIx, 1, blockProperties, 1, NULL, bufProp);
            GLErr();
            std::vector<char> s(bufProp[0]);
            glGetProgramResourceName(m_handle, GL_SHADER_STORAGE_BLOCK, blockIx, bufProp[0], NULL, s.data());
            GLErr();
            glShaderStorageBlockBinding(m_handle, blockIx, blockIx + numUnifBlocks);
            GLErr();
            m_buffers.emplace_back(blockIx + numUnifBlocks, string(s.data()), BufferTarget::ShaderStorage);
        }
        GLint numUniforms = 0;
        glGetProgramInterfaceiv(m_handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
        const GLenum properties[4] = {GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION};
        int samplerNum = 0;
        int imageNum = 0;

        for (int unif = 0; unif < numUniforms; ++unif) {
            GLint values[4];
            glGetProgramResourceiv(m_handle, GL_UNIFORM, unif, 4, properties, 4, NULL, values);
            GLErr();
            std::vector<char> nameData(values[2]);
            glGetProgramResourceName(m_handle, GL_UNIFORM, unif, nameData.size(), NULL, &nameData[0]);
            GLErr();
            int pos;
            auto type = static_cast<UniformType>(values[1]);
            GLErr();
            if (IsSampler(type)) {
                glUniform1i(values[3], samplerNum);
                GLErr();
                pos = samplerNum;
                samplerNum++;
            } else if (IsImage(type)) {
                glUniform1i(values[3], imageNum);
                GLErr();
                pos = imageNum;
                imageNum++;
            } else
                pos = values[3];
            m_uniforms.emplace_back(pos, string(nameData.data()), static_cast<UniformType>(values[1]),
                                    (values[0] >= 0) ? &m_buffers[values[0]] : nullptr);
        }
        Unbind();
    }

    int ShaderProgram::GetUniformLocation(const string &name) const noexcept {
        int res = -1;
        for (int i = 0; i < m_uniforms.size(); i++)
            if (m_uniforms[i].GetName() == name)
                res = m_uniforms[i].GetPos();
        return res;
    }

    template<>
    void ShaderProgram::Set(const string &name, const float &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform1f(pos, value); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const vec2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform2f(pos, value.x, value.y); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const vec3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform3f(pos, value.x, value.y, value.z); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const vec4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform4f(pos, value.x, value.y, value.z, value.w); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const double &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform1d(pos, value); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dvec2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform2d(pos, value.x, value.y); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dvec3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform3d(pos, value.x, value.y, value.z); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dvec4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform4d(pos, value.x, value.y, value.z, value.w); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const int &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform1i(pos, value); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const ivec2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform2i(pos, value.x, value.y); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const ivec3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform3i(pos, value.x, value.y, value.z); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const ivec4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform4i(pos, value.x, value.y, value.z, value.w); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const unsigned int &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform1ui(pos, value); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const uvec2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform2ui(pos, value.x, value.y); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const uvec3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform3ui(pos, value.x, value.y, value.z); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const uvec4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniform4ui(pos, value.x, value.y, value.z, value.w); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat2x3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2x3fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat2x4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2x4fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat3x2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3x2fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat3x4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3x4fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat4x2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4x2fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat4x3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4x3fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const mat4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4fv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat2x3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2x3dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat2x4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix2x4dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat3x2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3x2dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat3x4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix3x4dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat4x2 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4x2dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat4x3 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4x3dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    template<>
    void ShaderProgram::Set(const string &name, const dmat4 &value) {
        int pos = GetUniformLocation(name);
        if (pos < 0) return;
        glUniformMatrix4dv(pos, 1, false, value_ptr(value)); GLErr();
    }

    void ShaderProgram::SetTexture(const string &name, TextureBase *value) {
        for (int i = 0; i < m_uniforms.size(); i++)
            if (m_uniforms[i].GetName() == name) {
                if (m_uniforms[i].IsSampler())
                    value->SetActive(m_uniforms[i].GetPos());
                if (m_uniforms[i].IsImage())
                    value->BindImage(m_uniforms[i].GetPos());
                return;
            }
    }

    void ShaderProgram::SetBuffer(const string &name, BufferBase *value) {
        for (int i = 0; i < m_buffers.size(); i++)
            if (m_buffers[i].GetName() == name) {
                value->BindBase(m_buffers[i].GetBufferTarget(), m_buffers[i].GetPos());
                return;
            }
    }

    Material *ShaderProgram::CreateMaterial() const {
        Material *res = new Material();
        for (const auto &u : m_uniforms)
            res->AddUniform(u.GetName(), u.GetType());
        return res;
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(m_handle);
        GLErr();
    }
}
