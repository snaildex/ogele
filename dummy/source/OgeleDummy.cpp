#include <ogele.h>
#include <iostream>

using namespace ogele;

class RunnerApp : public Application {
	ShaderProgram *sh;

	Texture2D *tex;
	unique_ptr<RenderTarget> rt;
	unique_ptr<Camera> cam;
	unique_ptr<Terrain> m_terr;
	unique_ptr<DeferredPBRPipeline> m_pipeline;
	unique_ptr<GPUStopwatch<100>> m_fpsCounter;

	double m_sunRot;
	bool m_wireframe;

	void OnResize(const ivec2 &size) override {
		rt.reset(new RenderTarget(size, 1, TextureFormat::RGB8, false, true));
		cam->SetFrameSize(size);
		m_pipeline->Resize(size);
	};

	void OnCursorPos(const dvec2 &pos, const dvec2 &delta) override {
		cam->LookAround(delta, 0.4 * GetTimeDelta(), -1.5, 1.5);
	}

	void OnScroll(const dvec2 &offset) override {
		m_sunRot = clamp((m_sunRot + offset.y*0.1) / (2 * M_PI)) * 2 * M_PI;
	}

	void Start() override {
		rt = make_unique<RenderTarget>(GetResolution(), 1, TextureFormat::RGB8, false, true);
		cam = make_unique<PerspectiveCamera>(GetResolution(), 45, 1, 10000);
		m_fpsCounter = make_unique<GPUStopwatch<100>>();
		m_pipeline = make_unique<DeferredPBRPipeline>(GetResolution());
		m_terr = make_unique<Terrain>(ivec2(128, 128), 32);
		m_terr->Generate();
		m_terr->SetDrawRange(6);
		cam->SetLocalPos({ 0, 40, 0 });
		m_pipeline->SetFrameCamera(cam.get());
		Enable(Feature::SeamlessCubemap);
	}

	void Update() override {
		m_fpsCounter->Start();
		dvec3 delta;
		if (GetKey(Key::W)) delta += normalize(ProjectOnPlane(cam->Forward(), dvec3(0, 1, 0)));
		if (GetKey(Key::A)) delta -= normalize(ProjectOnPlane(cam->Right(), dvec3(0, 1, 0)));
		if (GetKey(Key::S)) delta -= normalize(ProjectOnPlane(cam->Forward(), dvec3(0, 1, 0)));
		if (GetKey(Key::D)) delta += normalize(ProjectOnPlane(cam->Right(), dvec3(0, 1, 0)));
		if (GetKey(Key::Space)) delta += dvec3(0, 1, 0);
		if (GetKey(Key::LShift)) delta -= dvec3(0, 1, 0);
		delta *= (float)GetTimeDelta() * 40;
		cam->SetLocalPos(cam->GetLocalPos() + delta);

		Enable(Feature::CullFace);
		Enable(Feature::DepthTest);

		ClearColor({ 0.4f, 0.6f, 0.8f, 1.0f });
		Clear(BufferBit::Color | BufferBit::Depth);

		dvec3 sunDir = glm::angleAxis(m_sunRot, normalize(dvec3(0, 0.3, 1))) * dvec3(1, 0, 0);
		m_pipeline->SetSunDir(sunDir);
		m_pipeline->Bind();
		Viewport({ 0, 0 }, GetResolution());
		ClearColor({ 0.4f, 0.6f, 0.8f, 1.0f });
		Clear(BufferBit::Color | BufferBit::Depth);
		SetPolygonMode(PolygonFace::FrontAndBack, m_wireframe ? PolygonMode::Line : PolygonMode::Fill);
		m_terr->Draw(cam.get());
		SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
		m_pipeline->Unbind();

		DrawBasis(cam->GetViewProjMatrix());
		Disable(Feature::CullFace);
		Disable(Feature::DepthTest);
		Disable(Feature::Blend);
		DrawTex((*m_pipeline->GetFinalBuffer())[0]);

		DrawBasis(cam->GetViewProjMatrix());
		m_fpsCounter->Stop();
		ImGui::SetNextWindowPos({ 0,0 });
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = { 0,0,0,0 };
		ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImGui::Text("Frame time: %3.1f ms", m_fpsCounter->GetTime() * 1000);
		ImGui::Checkbox("Wireframe", &m_wireframe);
		ImGui::End();
	}
};

int main(int argc, char *argv[]) {
	auto app = std::make_unique<RunnerApp>();
	app->Run();
	return 0;
}

