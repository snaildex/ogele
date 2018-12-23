#include <stdafx.h>
#include <ogele.h>

using namespace ogele;

class RunnerApp : public Application {
public:
	RunnerApp() : Application("Sandbox") {};
private:
	unique_ptr<RenderTarget> rt;
	Camera* cam;
	Terrain* m_terr;
	res_ptr<RenderPipeline> m_pip;
	Renderer* m_rend;

	double m_sunRot;

	void OnResize(const ivec2 &size) override {
		rt.reset(new RenderTarget(size, 1, TextureFormat::RGB8, TextureFilterMode::Nearest,
			TextureFilterMode::Nearest, false, true));
		cam->SetFrameSize(size);
	};

	void OnCursorPos(const dvec2 &pos, const dvec2 &delta) override {
		cam->LookAround(delta, 0.4 * GetTimeDelta(), -1.5, 1.5);
	}

	void OnScroll(const dvec2 &offset) override {
		m_sunRot = clamp((m_sunRot + offset.y*0.1) / (2 * M_PI)) * 2 * M_PI;
	}

	void Start() override {
		rt = make_unique<RenderTarget>(GetResolution(), 1, TextureFormat::RGB8, TextureFilterMode::Nearest,
			TextureFilterMode::Nearest, false, true);
		cam = Application::CreateActor("Camera")->AddComponent<PerspectiveCamera>(GetResolution(), 45.0, 0.1, 10000.0);
		m_terr = Application::CreateActor("Terrain")->AddComponent<Terrain>(ivec2(128, 128), 32);
		m_terr->Generate();
		m_terr->SetDrawRange(6);
		m_terr->GetActor()->AddTag("Opaque");
		cam->GetTransform()->SetLocalPos({ 0, 40, 0 });
		Enable(Feature::SeamlessCubemap);
		m_pip = Application::GetResourceByTag<RenderPipeline>("Main");
		m_rend = Application::CreateActor("Submarine")->AddComponent<Renderer>();
		m_rend->SetModel(Application::GetResourceByName<Model>("Submarine1"));
		m_sunRot = 0;
		m_rend->GetTransform()->CreateChildActor("Hull");
	}

	void Update() override {
		dvec3 delta;
		if (GetKey(Key::W)) delta += normalize(ProjectOnPlane(cam->GetTransform()->Forward(), dvec3(0, 1, 0)));
		if (GetKey(Key::A)) delta -= normalize(ProjectOnPlane(cam->GetTransform()->Right(), dvec3(0, 1, 0)));
		if (GetKey(Key::S)) delta -= normalize(ProjectOnPlane(cam->GetTransform()->Forward(), dvec3(0, 1, 0)));
		if (GetKey(Key::D)) delta += normalize(ProjectOnPlane(cam->GetTransform()->Right(), dvec3(0, 1, 0)));
		if (GetKey(Key::Space)) delta += dvec3(0, 1, 0);
		if (GetKey(Key::LShift)) delta -= dvec3(0, 1, 0);
		delta *= (float)GetTimeDelta() * 4;
		cam->GetTransform()->SetLocalPos(cam->GetTransform()->GetLocalPos() + delta);

		dvec3 sunDir = glm::angleAxis(m_sunRot, normalize(dvec3(0, 0.3, 1))) * dvec3(1, 0, 0);
		m_pip->GetMaterial()->Set<vec3>("sunDir", sunDir);
		m_pip->Render(cam);

		//GUI::Window("Debug",
		//	GUI::WindowFlags::Title |
		//	GUI::WindowFlags::Movable |
		//	GUI::WindowFlags::Scalable |
		//	GUI::WindowFlags::Border,
		//	{ 50,50 }, { 320,480 },
		//	[&] {
		//	GUI::Layout::RowDynamic(1);
		//	GUI::LabelColored("Whoooo", { 0.6,0.8,1.0,1.0 });
		//	//GUI::Image(Application::GetResourceByName<Texture2D>());
		//});

		Disable(Feature::DepthTest);
		Disable(Feature::CullFace);
		DrawBasis(cam->GetViewProjMatrix());
	}
};

int main(int argc, char *argv[]) {
	try {
		auto app = std::make_unique<RunnerApp>();
		app->Run();
	}
	catch (const ShaderCompileException& ex) {
		std::cout << ex.what();
		std::cin.get();
	}
	catch (const GLError& ex) {
		std::cout << ex.what();
		std::cin.get();
	}
}

