#include <stdafx.h>
#include <ogele.h>
#include "submarine.h"

using namespace ogele;

class Follower : public Component {
	ogele::Transform* m_target;
	double m_distance = 3;
	double m_height = 1;
public:
	std::string GetName() const override { return "Follower"; }
	Follower(ogele::Actor* actor) :Component(actor) {
		m_target = nullptr;
	}
	void SetTarget(ogele::Transform* target) noexcept { m_target = target; }
	ogele::Transform* GetTarget() const noexcept { return m_target; }

	void Update() override {
		dvec3 pos = m_target->GetLocalPos();
		pos -= m_target->Forward()*m_distance;
		pos += m_target->Up()*m_height;
		GetTransform()->SetLocalPos(glm::mix(GetTransform()->GetLocalPos(), pos, Application::GetTimeDelta() * 10));
		GetTransform()->SetLocalRot(glm::slerp(GetTransform()->GetLocalRot(), LookAtRotation(m_target->Forward(), m_target->Up()), Application::GetTimeDelta() * 10));
	}
};

class RunnerApp : public Application {
public:
	RunnerApp() : Application("Sandbox") {};
private:
	Camera* cam;
	res_ptr<RenderPipeline> m_pip;
	World* m_world;
	Actor* m_submarine;
	Terrain* m_terr;

	void OnResize(const ivec2 &size) override {
		cam->SetFrameSize(size);
		m_pip->Resize(size);
	};

	void OnCursorPos(const dvec2 &pos, const dvec2 &delta) override {
		/*if (GetMouseButton(MouseButton::Right) == KeyState::Press)
			cam->LookAround(delta, 0.4 * GetTimeDelta(), -1.5, 1.5);*/
	}

	void OnScroll(const dvec2 &offset) override {
	}

	void Start() override {
		m_world = Application::CreateWorld("Main");
		cam = m_world->CreateActor("Camera")->AddComponent<PerspectiveCamera>(GetResolution(), 45.0, 0.1, 10000.0);
		SetDebugCamera(cam);
		cam->GetTransform()->SetLocalPos({ 0, 0, 0 });
		Enable(Feature::SeamlessCubemap);
		m_pip = GetResourceByName<RenderPipeline>("Pipeline");
		m_pip->SetCamera(cam);
		m_pip->SetWorld(m_world);

		m_terr = m_world->CreateActor("Terrain")->AddComponent<Terrain>(ivec2(128, 128), 32);
		m_terr->Generate();
		m_terr->SetDrawRange(6);
		m_terr->GetActor()->AddTag("Opaque");
		Sparks* sparks = m_world->CreateActor("Sparks")->AddComponent<Sparks>(10);
		sparks->SetSprites(Application::GetResourceByName<Texture2DArray>("Sparks"));
		Particles* particles = m_world->CreateActor("Bubbles")->AddComponent<Particles>(50 * 1024);
		particles->SetSprites(Application::GetResourceByName<Texture2DArray>("Bubbles"));

		m_submarine = GetResourceByName<Model>("Submarine1")->Instantiate();
		m_world->AddRoot(m_submarine->GetTransform());
		m_submarine->AddComponent<Submarine>();
		cam->GetActor()->AddComponent<Follower>()->SetTarget(m_submarine->GetTransform());
		m_world->SetGravity({ 0,0,0 });
		m_submarine->GetTransform()->SetLocalPos({ 0,0,0 });
	}

	void Update() override {
		m_world->StepPhysics(GetTimeDelta());
		m_world->Update();

		cam->UpdateMaterial();

		m_pip->Render();
		Disable(Feature::DepthTest);
		Disable(Feature::CullFace);
		Enable(Feature::Blend);
		BlendFunc(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);
		m_world->OnGui();
		m_terr->GUI();
	}
};

int main(int argc, char *argv[]) {
	auto app = std::make_unique<RunnerApp>();
	app->Run();
}

