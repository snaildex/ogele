#pragma once
namespace ogele {
	class TextureBase;
	class Material;

	class ShaderProgram : public HandleBase<GLenum>, public Resource {
	public:
		class Proxy;

		class BufferInfo {
			int m_pos;
			std::string m_name;
			BufferTarget m_bufferTarget;

		public:
			int GetPos() const noexcept { return m_pos; }

			const std::string &GetName() const noexcept { return m_name; }

			BufferTarget GetBufferTarget() const noexcept { return m_bufferTarget; }

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
			int GetPos() const noexcept { return m_pos; }

			const std::string &GetName() const noexcept { return m_name; }

			UniformType GetType() const noexcept { return m_type; }

			const BufferInfo *GetBuffer() const noexcept { return m_buffer; }

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

		int GetUniformLocation(const std::string &name) const noexcept;

	public:
		ShaderProgram();

		void AttachStage(ShaderStage *stage) {
			glAttachShader(m_handle, stage->GetHandle());
			GLErr();
		}

		void DetachStage(ShaderStage *stage) {
			glDetachShader(m_handle, stage->GetHandle());
			GLErr();
		}

		void Bind() {
			glUseProgram(m_handle);
			GLErr();
		}

		void Unbind() {
			glUseProgram(0);
			GLErr();
		}

		void Link();

		template<typename T>
		void Set(const std::string &name, const T &value);

		void SetTexture(const std::string &name, TextureBase *value);

		void SetBuffer(const std::string &name, BufferBase *value);

		void bDispatchCompute(const glm::ivec3 &groupNum) const {
			glDispatchCompute(groupNum.x, groupNum.y, groupNum.z);
			GLErr();
		}

		Material *CreateMaterial() const;

		~ShaderProgram();
	};

	class ShaderProgramCollection {
		Entity(ShaderProgramCollection);
		std::vector<res_ptr<ShaderProgram>> m_shaders;
	public:
		ShaderProgramCollection() = default;
		void FindResouces(const std::vector<std::string>& tags);

		const std::vector<res_ptr<ShaderProgram>>& GetShaders() const noexcept { return m_shaders; }
		void Add(res_ptr<ShaderProgram> sh) noexcept { m_shaders.push_back(sh); }
		res_ptr<ShaderProgram> Get(const std::vector<std::string>& tags) const;
	};

	ShaderProgram *CreateShaderProgram(const std::string &name, const std::string &vertex, const std::string &fragment);

	ShaderProgram *CreateShaderProgram(const std::string &name, const std::string &compute);
}