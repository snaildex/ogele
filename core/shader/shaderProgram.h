//
// Created by ??????? on 21.07.2018.
//

#ifndef OGELE_SHADERPROGRAM_H
#define OGELE_SHADERPROGRAM_H
namespace ogele {
    class TextureBase;

    class Material;

    class ShaderProgram : public HandleBase<GLenum>, public Resource {
    public:

        class BufferInfo {
            int m_pos;
            std::string m_name;
            BufferTarget m_bufferTarget;

        public:
            inline int GetPos() const noexcept { return m_pos; }

            inline const std::string &GetName() const noexcept { return m_name; }

            inline BufferTarget GetBufferTarget() const noexcept { return m_bufferTarget; }

            BufferInfo(int pos, const std::string &name, BufferTarget bufferTarget) {
                m_pos = pos;
                m_name = name;
                m_bufferTarget = bufferTarget;
            }
        };

        static bool IsSampler(UniformType type);

        static bool IsImage(UniformType type);

        class UniformInfo {
            int m_pos;
            std::string m_name;
            UniformType m_type;
            BufferInfo *m_buffer;

        public:
            inline int GetPos() const noexcept { return m_pos; }

            inline const std::string &GetName() const noexcept { return m_name; }

            inline UniformType GetType() const noexcept { return m_type; }

            inline const BufferInfo *GetBuffer() const noexcept { return m_buffer; }

            bool IsSampler() const noexcept { return ShaderProgram::IsSampler(m_type); }

            bool IsImage() const noexcept { return ShaderProgram::IsImage(m_type); }

            UniformInfo(int pos, const std::string &name, UniformType type, BufferInfo *buffer) {
                m_pos = pos;
                m_name = name;
                m_type = type;
                m_buffer = buffer;
            }
        };

    private:
        std::vector <BufferInfo> m_buffers;
        std::vector <UniformInfo> m_uniforms;

        int GetUniformLocation(const string &name) const noexcept;

    public:
        ShaderProgram();

        inline void AttachStage(ShaderStage *stage) {
            glAttachShader(m_handle, stage->GetHandle());
            GLErr();
        }

        inline void DetachStage(ShaderStage *stage) {
            glDetachShader(m_handle, stage->GetHandle());
            GLErr();
        }

        inline void Bind() {
            glUseProgram(m_handle);
            GLErr();
        }

        inline void Unbind() {
            glUseProgram(0);
            GLErr();
        }

        void Link();

        template<typename T>
        void Set(const string &name, const T &value);

        void SetTexture(const string &name, TextureBase *value);

        void SetBuffer(const string &name, BufferBase *value);

        inline void bDispatchCompute(const ivec3 &groupNum) const {
            glDispatchCompute(groupNum.x, groupNum.y, groupNum.z);
            GLErr();
        }

        Material *CreateMaterial() const;

        ~ShaderProgram();
    };

    ShaderProgram *CreateShaderProgram(const string &vertex, const string &fragment);

    ShaderProgram *CreateShaderProgram(const string &compute);
}
#endif //OGELE_SHADERPROGRAM_H
