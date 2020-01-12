#include <stdafx.h>
#include <ogele.h>

using namespace ogele;

class RunnerApp : public Application {
public:
	RunnerApp() : Application("Sandbox") {};
private:
	Camera* cam;
	Terrain* m_terr;
	res_ptr<RenderPipeline> m_pip;
	Renderer* m_rend;
	World* m_world;

	double m_sunRot;

	void OnResize(const ivec2 &size) override {
		cam->SetFrameSize(size);
		//m_pip->Resize(size);
	};

	void OnCursorPos(const dvec2 &pos, const dvec2 &delta) override {
		if (GetMouseButton(MouseButton::Right) == KeyState::Press)
			cam->LookAround(delta, 0.4 * GetTimeDelta(), -1.5, 1.5);
	}

	void OnScroll(const dvec2 &offset) override {
		m_sunRot = clamp((m_sunRot + offset.y*0.1) / (2 * M_PI)) * 2 * M_PI;
	}

	void Start() override {
		m_world = Application::CreateWorld("Main");
		cam = m_world->CreateActor("Camera")->AddComponent<PerspectiveCamera>(GetResolution(), 45.0, 0.1, 10000.0);
		m_terr = m_world->CreateActor("Terrain")->AddComponent<Terrain>(ivec2(128, 128), 32);
		m_terr->Generate();
		m_terr->SetDrawRange(6);
		m_terr->GetActor()->AddTag("Opaque");
		cam->GetTransform()->SetLocalPos({ 0, 40, 0 });
		Enable(Feature::SeamlessCubemap);
		m_pip = GetResourceByName<RenderPipeline>("Pipeline");
		m_pip->SetCamera(cam);
		m_pip->SetWorld(m_world);
		m_rend = m_world->CreateActor("Submarine")->AddComponent<Renderer>();
		m_rend->SetModel(GetResourceByName<Model>("Submarine1"));
		m_sunRot = 0;
		m_world->SetGravity({ 0,0,0 });
		Rigidbody::ConstructionInfo info{};
		info.Mass = 10;
		info.LocalInertia = { 1,1,1 };
		m_world->AddRigidbody(m_rend->GetActor()->AddComponent<Rigidbody>(info));
	}
	glm::vec3 test;
	void Update() override {
		m_world->StepPhysics(GetTimeDelta());

		dvec3 delta;
		/*if (GetKey(Key::W)) delta += normalize(ProjectOnPlane(cam->GetTransform()->Forward(), dvec3(0, 1, 0)));
		if (GetKey(Key::A)) delta -= normalize(ProjectOnPlane(cam->GetTransform()->Right(), dvec3(0, 1, 0)));
		if (GetKey(Key::S)) delta -= normalize(ProjectOnPlane(cam->GetTransform()->Forward(), dvec3(0, 1, 0)));
		if (GetKey(Key::D)) delta += normalize(ProjectOnPlane(cam->GetTransform()->Right(), dvec3(0, 1, 0)));*/
		if (GetKey(Key::Space)) delta += dvec3(0, 1, 0);
		if (GetKey(Key::LShift)) delta -= dvec3(0, 1, 0);
		delta *= (float)GetTimeDelta() * 4;
		cam->GetTransform()->SetLocalPos(cam->GetTransform()->GetLocalPos() + delta);
		cam->GetTransform()->SetLocalPos(m_rend->GetTransform()->GetLocalPos() - cam->GetTransform()->Forward()*3.0);
		cam->UpdateMaterial();

		dvec3 sunDir = glm::angleAxis(m_sunRot, normalize(dvec3(0, 0.3, 1))) * dvec3(1, 0, 0);
		m_pip->GetMaterial()->Set<vec3>("sunDir", sunDir);
		m_pip->Render();

		/*GUI::Window("Kek", [&] {
			GUI::Text("M %s", glm::to_string(m_rend->GetTransform()->GetMatrix()).data());
		});*/
		//m_pip->GUI();
		//m_terr->GUI();

		Disable(Feature::DepthTest);
		Disable(Feature::CullFace);
		DrawBasis(cam->GetViewProjMatrix());
	}
};

int main(int argc, char *argv[]) {
	auto app = std::make_unique<RunnerApp>();
	app->Run();
}

