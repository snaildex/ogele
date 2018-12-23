#pragma once
using namespace glm;
using namespace std;
namespace ogele {
    class Material : Resource {
        class UniformBase {
        protected:
            UniformBase() = default;

        public:
            virtual void Apply(const std::string &name, ShaderProgram *sh) const = 0;

            virtual std::string GetTypeName() const noexcept = 0;

            virtual std::string PrintValue() const noexcept = 0;

            virtual void GUI(const std::string &name) = 0;

            virtual ~UniformBase() = default;

            virtual void TryCopyFrom(UniformBase *other) = 0;
        };

        template<typename valueType>
        class UniformValue : public UniformBase {
			valueType m_value;
        public:
             const valueType &GetValue() const noexcept { return m_value; }

             void SetValue(const valueType &value) noexcept { m_value = value; }

            void Apply(const std::string &name, ShaderProgram *sh) const override { sh->Set(name, m_value); }

            std::string GetTypeName() const noexcept override { return typeid(valueType).name(); };

            std::string PrintValue() const noexcept override { return to_string(m_value); };

            UniformValue() = default;

            UniformValue(const valueType &val) { m_value = val; }

            void GUI(const std::string &name) override { PropertyGUI<valueType>(name, &m_value); }

            void TryCopyFrom(UniformBase *other) override {
                UniformValue<valueType> *cother = dynamic_cast<UniformValue<valueType> *>(other);
                if (cother)
                    m_value = cother->m_value;
            }
        };

        class UniformTexture : public UniformBase {
            TextureBase *m_value;
        public:
             TextureBase *GetValue() const noexcept { return m_value; }

             void SetValue(TextureBase *value) noexcept { m_value = value; }

            void Apply(const std::string &name, ShaderProgram *sh) const override { sh->SetTexture(name, m_value); }

            std::string GetTypeName() const noexcept override { return typeid(m_value).name(); };

            std::string PrintValue() const noexcept override { return m_value->GetName(); };

            UniformTexture() = default;

            UniformTexture(TextureBase *val) { m_value = val; }

            void GUI(const std::string &name) override {
            }

            void TryCopyFrom(UniformBase *other) override {
                UniformTexture *cother = dynamic_cast<UniformTexture *>(other);
                if (cother)
                    m_value = cother->m_value;
            }
        };

        class UniformBuffer : public UniformBase {
            BufferBase *m_value;
        public:
             BufferBase *GetValue() const noexcept { return m_value; }

             void SetValue(BufferBase *value) noexcept { m_value = value; }

            void Apply(const std::string &name, ShaderProgram *sh) const override { sh->SetBuffer(name, m_value); }

            std::string GetTypeName() const noexcept override { return typeid(m_value).name(); };

            std::string PrintValue() const noexcept override { return "Buffer"; };

            UniformBuffer() = default;

            UniformBuffer(BufferBase *val) { m_value = val; }

            void GUI(const std::string &name) override {
            }

            void TryCopyFrom(UniformBase *other) override {
                UniformBuffer *cother = dynamic_cast<UniformBuffer *>(other);
                if (cother)
                    m_value = cother->m_value;
            }
        };

        mutable std::map<std::string, std::unique_ptr<UniformBase>> m_data;
    public:
        void AddUniform(const std::string &name, UniformType type);

		template<typename T>
		 void Set(const std::string &name, const T &value) noexcept {
            m_data[name] = unique_ptr<UniformBase>(new UniformValue<T>(value));
        }

        template<typename T>
         T
        Get(const std::string &name) const { return dynamic_cast<UniformValue<T> *>(m_data[name].get())->GetValue(); }

         void SetTexture(const std::string &name, TextureBase *value) noexcept {
            m_data[name] = std::unique_ptr<UniformBase>(new UniformTexture(value));
        }

         TextureBase *
        GetTexture(const std::string &name) const { return dynamic_cast<UniformTexture *>(m_data[name].get())->GetValue(); }

         void SetBuffer(const std::string &name, BufferBase *value) noexcept {
            m_data[name] = std::unique_ptr<UniformBase>(new UniformBuffer(value));
        }

         BufferBase *
        GetBuffer(const std::string &name) const { return dynamic_cast<UniformBuffer *>(m_data[name].get())->GetValue(); }

        void Apply(ShaderProgram *sh);

        void GUI();

        void FetchValues(Material *mat);
    };

}
