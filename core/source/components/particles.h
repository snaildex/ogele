#pragma once
namespace ogele {

	class Particles : public Renderable {
		struct Particle {
			dvec3 Position;
			double Life;
			dvec3 Speed;
			double Size;
			dvec3 Force;
			double Rotation;
			double FadeStart;
			double SizeFade;
			double AlphaFade;
			double TexIndex;
			dvec4 Color;
		};

	public:
		class Emitter : public Component {
			Particles* m_particles;
			double m_life;
			double m_fadeDelay;
			double m_startSize;
			double m_endSize;
			double m_spawnRate;
			bool m_randomRotation;
			double m_spawnTime;

			Particle m_sample;

		public:
			Emitter(Actor* actor, Particles* particles);
			~Emitter();
			virtual std::string GetName() const { return "Particle emitter"; };

			const Particle& GetSample() const noexcept { return m_sample; }
			uint GetSpawnCount();

			void SetLife(double value) noexcept;
			void SetFadeDelay(double value) noexcept;
			void SetStartSize(double value) noexcept;
			void SetEndSize(double value) noexcept;
			void SetRandomRotation(bool value) noexcept;
			void SetSpeed(const dvec3& value) noexcept;
			void SetForce(const dvec3& value) noexcept;
			void SetColor(const dvec4& value) noexcept;
			void SetSpawnRate(double value) noexcept;
		};

	private:
		ShaderProgramCollection m_shaders;
		static ShaderProgram* m_generator;
		std::unique_ptr<DoubleBuffer<Particle>> m_buffer;
		std::unique_ptr<DoubleBuffer<Particle>> m_samples;
		std::unique_ptr<DoubleBuffer<dmat4>> m_emitterTransforms;
		std::unique_ptr<UnsignedIntBuffer<uint>> m_emitterIndecies;
		std::unique_ptr<UnsignedIntBuffer<uint>> m_counter;
		std::unique_ptr<Material> m_material;
		Particle* m_sampleData;
		dmat4* m_emitterTransformData;
		uint* m_emitterIndeciesData;

		std::unique_ptr<VertexArray> m_vao;
		std::list<Emitter*> m_emitters;
		int m_count;
		const int m_maxEmitters = 4096;

		void AddEmitter(Emitter* emitter);
		void RemoveEmitter(Emitter* emitter);
		void RebuildSamples();

	public:
		static void Init();
		static void Shutdown();

		Particles(Actor* actor, int count);
		~Particles();
		void SetSprites(res_ptr<Texture2DArray> value) noexcept;

		virtual std::string GetName() const override { return "Particle system"; }
		virtual void Update() override;
		virtual void Render(const Camera* camera, const std::vector<std::string>& tags, const ogele::Material* material = nullptr) const override;
	};

}