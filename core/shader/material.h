//
// Created by ??????? on 22.07.2018.
//

#ifndef OGELE_MATERIAL_H
#define OGELE_MATERIAL_H
namespace ogele {

    class Material : Resource {
        class UniformBase {
        protected:
            UniformBase() = default;

        public:
            virtual void Apply(const string &name, ShaderProgram *sh) const = 0;

            virtual string GetTypeName() const noexcept = 0;

            virtual string PrintValue() const noexcept = 0;

            virtual void GUI(const string &name) = 0;

            virtual ~UniformBase() = default;

            virtual void TryCopyFrom(UniformBase *other) = 0;
        };

        template<typename T>
        class UniformValue : public UniformBase {
            T m_value;
        public:
            inline const T &GetValue() const noexcept { return m_value; }

            inline void SetValue(const T &value) noexcept { m_value = value; }

            void Apply(const string &name, ShaderProgram *sh) const override { sh->Set(name, m_value); }

            string GetTypeName() const noexcept override { return typeid(T).name(); };

            string PrintValue() const noexcept override { return to_string(m_value); };

            UniformValue() = default;

            UniformValue(const T &val) { m_value = val; }

            void GUI(const string &name) override { PropertyGUI<T>(name, &m_value); }

            void TryCopyFrom(UniformBase *other) override {
                UniformValue<T> *cother = dynamic_cast<UniformValue<T> *>(other);
                if (cother)
                    m_value = cother->m_value;
            }
        };

        class UniformTexture : public UniformBase {
            TextureBase *m_value;
        public:
            inline TextureBase *GetValue() const noexcept { return m_value; }

            inline void SetValue(TextureBase *value) noexcept { m_value = value; }

            void Apply(const string &name, ShaderProgram *sh) const override { sh->SetTexture(name, m_value); }

            string GetTypeName() const noexcept override { return typeid(m_value).name(); };

            string PrintValue() const noexcept override { return m_value->GetName(); };

            UniformTexture() = default;

            UniformTexture(TextureBase *val) { m_value = val; }

            void GUI(const string &name) override { PropertyGUI<TextureBase *>(name, &m_value); }

            void TryCopyFrom(UniformBase *other) override {
                UniformTexture *cother = dynamic_cast<UniformTexture *>(other);
                if (cother)
                    m_value = cother->m_value;
            }
        };

        class UniformBuffer : public UniformBase {
            BufferBase *m_value;
        public:
            inline BufferBase *GetValue() const noexcept { return m_value; }

            inline void SetValue(BufferBase *value) noexcept { m_value = value; }

            void Apply(const string &name, ShaderProgram *sh) const override { sh->SetBuffer(name, m_value); }

            string GetTypeName() const noexcept override { return typeid(m_value).name(); };

            string PrintValue() const noexcept override { return "Buffer"; };

            UniformBuffer() = default;

            UniformBuffer(BufferBase *val) { m_value = val; }

            void GUI(const string &name) override { PropertyGUI<BufferBase *>(name, &m_value); }

            void TryCopyFrom(UniformBase *other) override {
                UniformBuffer *cother = dynamic_cast<UniformBuffer *>(other);
                if (cother)
                    m_value = cother->m_value;
            }
        };

        mutable map <string, unique_ptr<UniformBase>> m_data;
    public:
        void AddUniform(const string &name, UniformType type);

        template<typename T>
        inline void Set(const string &name, const T &value) noexcept {
            m_data[name] = unique_ptr<UniformBase>(new UniformValue<T>(value));
        }

        template<typename T>
        inline T
        Get(const string &name) const { return dynamic_cast<UniformValue<T> *>(m_data[name].get())->GetValue(); }

        inline void SetTexture(const string &name, TextureBase *value) noexcept {
            m_data[name] = unique_ptr<UniformBase>(new UniformTexture(value));
        }

        inline TextureBase *
        GetTexture(const string &name) const { return dynamic_cast<UniformTexture *>(m_data[name].get())->GetValue(); }

        inline void SetBuffer(const string &name, BufferBase *value) noexcept {
            m_data[name] = unique_ptr<UniformBase>(new UniformBuffer(value));
        }

        inline BufferBase *
        GetBuffer(const string &name) const { return dynamic_cast<UniformBuffer *>(m_data[name].get())->GetValue(); }

        void Apply(ShaderProgram *sh);

        void GUI();

        void FetchValues(Material *mat);
    };

}
#endif //OGELE_MATERIAL_H
