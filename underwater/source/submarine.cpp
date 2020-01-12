#include <stdafx.h>
#include <ogele.h>
#include "submarine.h"

using namespace ogele;

class Propeller : public Component {
	Submarine* m_sub;
	Particles::Emitter* m_emitter;
public:
	std::string GetName() const override { return "Submarine propeller"; }
	Propeller(Actor* actor, Submarine* sub) : Component(actor) {
		m_sub = sub;
		Particles* bubbles = GetTransform()->GetWorld()->GetRoot("Bubbles")->GetActor()->GetComponent<Particles>();
		m_emitter = GetTransform()->CreateChildActor("BubbleEmitter")->AddComponent<Particles::Emitter>(bubbles);
		m_emitter->GetTransform()->SetLocalScale({ 5,5,5 });
		m_emitter->SetLife(1);
		m_emitter->SetFadeDelay(0.6);
		m_emitter->SetForce({ 0,1,0 });
		m_emitter->SetStartSize(0.01);
		m_emitter->SetEndSize(0.01);
		m_emitter->SetSpawnRate(0.01);
	}
	void Update() override {
		double rate = m_sub->GetCurrentSpeed() / m_sub->GetStats().Speed;
		GetTransform()->Rotate(dvec3(-10 * Application::GetTimeDelta() * rate, 0, 0));
		m_emitter->SetActive(rate > 0.2);
		m_emitter->SetSpawnRate(pow(0.1, 2 * rate));
	}
};

Submarine::Submarine(Actor * actor) : Component(actor)
{
	m_stats.Speed = 2;
	m_stats.Turn = 1;
	m_curSpeed = 0;
	auto props = GetTransform()->FindChilds([](Transform* tr) { return tr->GetActor()->GetName().find("Propeller") != tr->GetActor()->GetName().npos; });
	for (auto p : props) p->GetActor()->AddComponent<Propeller>(this);
}

void Submarine::Update()
{
	dvec3 rot{ 0 };
	rot.x = Application::GetInstance()->GetJoystickAxis(0, 1);
	rot.y = -Application::GetInstance()->GetJoystickAxis(0, 2);
	rot.z = -Application::GetInstance()->GetJoystickAxis(0, 0);
	/*if (Application::GetInstance()->GetKey(Key::W)) rot.x = 1;
	if (Application::GetInstance()->GetKey(Key::S)) rot.x = -1;
	if (Application::GetInstance()->GetKey(Key::A)) rot.z = 1;
	if (Application::GetInstance()->GetKey(Key::D)) rot.z = -1;
	if (Application::GetInstance()->GetKey(Key::Q)) rot.y = 1;
	if (Application::GetInstance()->GetKey(Key::E)) rot.y = -1;*/

	double jitter = 0.02 * m_curSpeed;
	rot.x += jitter * SimplexNoise({ Application::GetTime(), 13.76 });
	rot.y += jitter * SimplexNoise({ Application::GetTime(), 1938.337 });

	GetTransform()->Rotate(rot * m_stats.Turn * Application::GetTimeDelta());

	//if (Application::GetInstance()->GetKey(Key::X)) m_curSpeed += m_stats.Speed * Application::GetTimeDelta() * 0.25;
	//if (Application::GetInstance()->GetKey(Key::Z)) m_curSpeed -= m_stats.Speed * Application::GetTimeDelta() * 0.25;
	m_curSpeed = m_stats.Speed * (Application::GetInstance()->GetJoystickAxis(0, 3) + 1)*0.5;
	m_curSpeed = glm::clamp(m_curSpeed, 0.0, m_stats.Speed);
	GetTransform()->MoveLocal(GetTransform()->Forward() * m_curSpeed * Application::GetTimeDelta());
}

void Submarine::OnGui()
{
	dvec3 fwd = normalize(ProjectOnPlane(GetTransform()->Forward(), { 0,1,0 }));
	Application::DrawRay(GetTransform()->GetPos(), fwd, { 1,1,1 });
	dvec2 p = Application::WorldToScreen(GetTransform()->GetPos() + GetTransform()->Forward() * 40.0);
	Application::DrawTex(Application::GetResourceByName<Texture2D>("Circle").get(), { 32,32 }, p);
	//GUI::WindowFlags flags = GUI::WindowFlags::NoInputs | GUI::WindowFlags::NoMove | GUI::WindowFlags::NoCollapse;
	//GUI::Window("Submarine1", []() {}, flags);
	GUI::ProgressBar(0.63, "Hull");
	GUI::ProgressBar(0.63, "Shield");
}
