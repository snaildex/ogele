#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	const string GeneratorSource = R"(
		#version 430 core

		layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

		struct Particle {
			dvec4 PositionLife;
			dvec4 SpeedSize;
			dvec4 ForceRotation;
			dvec4 Props; //FadeStart, SizeFade, AlphaFade, TexIndex
			dvec4 Color;
		};

		layout(std430, binding = 0) buffer ParticleData{Particle Particles[];};
		layout(std430, binding = 1) buffer ParticleSamples{Particle Samples[];};
		layout(std430, binding = 3) buffer EmitterTransforms{dmat4 Transforms[];};
		layout(std430, binding = 4) buffer EmitterIndecies{uint Indecies[];};
		layout(binding = 5) uniform atomic_uint Counter;

		uniform double TimeDelta;
		uniform uint GenerateCount;

		float rand(vec3 co) {
			return cos(sin(dot(co,vec3(12.9898,78.233,1999.228))) * 43758.5453);
		}

		void main()
		{
			if(Particles[gl_GlobalInvocationID.x].PositionLife.w <= 0)
			{
				uint pId = atomicCounterIncrement(Counter);
				if(pId < GenerateCount)
				{
					uint emId = Indecies[pId];
					Particle s = Samples[emId];
					dmat4 tr = Transforms[emId];
					dvec4 pos;
					pos.x = double(rand(vec3(gl_GlobalInvocationID.x, 229.31, TimeDelta)));
					pos.y = double(rand(vec3(gl_GlobalInvocationID.x, 4779.978, TimeDelta)));
					pos.z = double(rand(vec3(gl_GlobalInvocationID.x, 12121.46463, TimeDelta)));
					pos.w=1;
					pos = tr*pos;
					pos/=pos.w;
					s.PositionLife.xyz=pos.xyz;
					Particles[gl_GlobalInvocationID.x] = s;
				}
			}
		}
	)";

	ShaderProgram* Particles::m_generator;

	void Particles::Init()
	{
		m_generator = CreateShaderProgram("ParticleGenerator", GeneratorSource);
		m_generator->AddTag("Internal");
	}
	void Particles::Shutdown()
	{
		delete m_generator;
	}
	Particles::Particles(Actor* actor, int count) : Renderable(actor)
	{
		GetActor()->AddTag("Particles");
		m_shaders.FindResouces({ "Particles" });
		m_vao = make_unique<VertexArray>(Topology::Points);
		unique_ptr<Particle[]> data = make_unique<Particle[]>(count);
		memset(data.get(), 0, sizeof(Particle) * count);
		m_buffer = make_unique<DoubleBuffer<Particle>>(BufferFlags::None, count, data.get());

		m_samples = make_unique<DoubleBuffer<Particle>>(BufferFlags::MapPersistent | BufferFlags::MapWrite, m_maxEmitters, nullptr);
		m_samples->Bind();
		m_sampleData = m_samples->bMapRange(0, m_maxEmitters, MappingOptionsFlags::FlushExplicit);
		m_samples->Unbind();

		m_emitterTransforms = make_unique<DoubleBuffer<dmat4>>(BufferFlags::MapPersistent | BufferFlags::MapWrite, m_maxEmitters, nullptr);
		m_emitterTransforms->Bind();
		m_emitterTransformData = m_emitterTransforms->bMapRange(0, m_maxEmitters, MappingOptionsFlags::FlushExplicit);
		m_emitterTransforms->Unbind();

		m_emitterIndecies = make_unique<UnsignedIntBuffer<uint>>(BufferFlags::MapPersistent | BufferFlags::MapWrite, count, nullptr);
		m_emitterIndecies->Bind();
		m_emitterIndeciesData = m_emitterIndecies->bMapRange(0, count, MappingOptionsFlags::FlushExplicit);
		m_emitterIndecies->Unbind();

		m_counter = make_unique<UnsignedIntBuffer<uint>>(BufferFlags::None, 1, nullptr);
		m_counter->Bind();
		m_counter->bClearRange(TextureFormat::R32UI, 0, 1, PixelDataFormat::RedInteger, 0);
		m_counter->Unbind();

		m_count = count;

		m_material = make_unique<Material>();
		m_material->SetBuffer("ParticleData", m_buffer.get());
		m_material->SetBuffer("ParticleSamples", m_samples.get());
		m_material->SetBuffer("EmitterTransforms", m_emitterTransforms.get());
		m_material->SetBuffer("EmitterIndecies", m_emitterIndecies.get());
		m_material->SetBuffer("AtomicCounter5", m_counter.get());
	}
	Particles::~Particles()
	{
		m_samples->Bind();
		m_samples->bUnmap();
		m_samples->Unbind();
		m_emitterTransforms->Bind();
		m_emitterTransforms->bUnmap();
		m_emitterTransforms->Unbind();
		auto em = m_emitters;
		for (const auto& e : em) DeleteComponent(e);
	}
	void Particles::SetSprites(res_ptr<Texture2DArray> value) noexcept
	{
		m_material->SetTexture("Sprites", value.get());
	}
	void Particles::Update()
	{
		int i = 0;
		int j = 0;
		for (const auto& e : m_emitters)
			if (e->IsActive())
			{
				for (int c = e->GetSpawnCount(); c > 0; c--) m_emitterIndeciesData[j++] = i;
				m_emitterTransformData[i++] = e->GetTransform()->GetMatrix();
			}
		if (j == 0) return;

		m_emitterTransforms->Bind();
		m_emitterTransforms->bFlushRange(0, i);
		m_emitterTransforms->Unbind();
		m_emitterIndecies->Bind();
		m_emitterIndecies->bFlushRange(0, j);
		m_emitterIndecies->Unbind();
		m_counter->Bind();
		m_counter->bClearRange(TextureFormat::R32UI, 0, 1, PixelDataFormat::RedInteger, 0);
		m_counter->Unbind();

		RebuildSamples();

		m_generator->Bind();
		Application::GetMaterial()->Apply(m_generator);
		m_material->Apply(m_generator);
		m_generator->Set<uint>("GenerateCount", j);
		m_generator->bDispatchCompute({ m_count / 1024,1,1 });
		m_generator->Unbind();
	}
	void Particles::Render(const Camera* camera, const std::vector<std::string>& tags, const ogele::Material* material) const
	{
		auto sh = m_shaders.Get(tags);
		sh->Bind();
		Application::GetMaterial()->Apply(sh.get());
		camera->GetMaterial()->Apply(sh.get());
		if (material) material->Apply(sh.get());
		m_material->Apply(sh.get());
		m_vao->Bind();
		m_vao->bDraw(0, m_count);
		sh->Unbind();
	}
	void Particles::AddEmitter(Emitter* emitter)
	{
		m_emitters.push_back(emitter);
	}
	void Particles::RemoveEmitter(Emitter* emitter)
	{
		m_emitters.remove(emitter);
	}
	void Particles::RebuildSamples()
	{
		int i = 0;
		for (const auto& e : m_emitters) m_sampleData[i++] = e->GetSample();
		m_samples->Bind();
		m_samples->bFlushRange(0, i);
		m_samples->Unbind();
	}
	Particles::Emitter::Emitter(Actor* actor, Particles* particles) :Component(actor)
	{
		m_particles = particles;
		m_particles->AddEmitter(this);
		m_spawnTime = 0;
		m_sample = Particle{};
		m_sample.Color = { 1,1,1,1 };
		SetLife(5);
		SetFadeDelay(4);
		SetStartSize(0.3);
		SetEndSize(0.3);
		SetRandomRotation(false);
		SetSpawnRate(0.1);
	}

	uint Particles::Emitter::GetSpawnCount()
	{
		m_spawnTime += Application::GetTimeDelta();
		uint res = floor(m_spawnTime / m_spawnRate);
		m_spawnTime -= res * m_spawnRate;
		return res;
	}

	void Particles::Emitter::SetLife(double value) noexcept {
		m_life = value;
		m_sample.Life = value;
		m_sample.AlphaFade = 1.0 / (m_life - m_fadeDelay);
		m_sample.SizeFade = (m_endSize - m_startSize) / (m_life);
	}
	void Particles::Emitter::SetFadeDelay(double value) noexcept {
		m_fadeDelay = value;
		m_sample.FadeStart = m_life - value;
		m_sample.AlphaFade = 1.0 / (m_life - m_fadeDelay);
	}
	void Particles::Emitter::SetStartSize(double value) noexcept {
		m_startSize = value;
		m_sample.Size = value;
		m_sample.SizeFade = (m_endSize - m_startSize) / (m_life);
	}
	void Particles::Emitter::SetEndSize(double value) noexcept {
		m_endSize = value;
		m_sample.SizeFade = (m_endSize - m_startSize) / (m_life);
	}
	void Particles::Emitter::SetRandomRotation(bool value) noexcept {
		m_randomRotation = value;
	}
	void Particles::Emitter::SetSpeed(const dvec3& value) noexcept
	{
		m_sample.Speed = value;
	}
	void Particles::Emitter::SetForce(const dvec3& value) noexcept
	{
		m_sample.Force = value;
	}
	void Particles::Emitter::SetColor(const dvec4& value) noexcept
	{
		m_sample.Color = value;
	}
	void Particles::Emitter::SetSpawnRate(double value) noexcept {
		m_spawnRate = value;
	}

	Particles::Emitter::~Emitter()
	{
		m_particles->RemoveEmitter(this);
	}
}