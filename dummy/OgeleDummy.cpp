#include "../core/ogele.h"

using namespace ogele;

class RunnerApp : public Application {
    ShaderProgram *sh;

    Texture2D *tex;
    unique_ptr<RenderTarget> rt;
    unique_ptr<Camera> cam;
    unique_ptr<Terrain> m_terr;
    unique_ptr<DeferredPBRPipeline> m_pipeline;

    float m_sunRot;

    void OnResize(const ivec2 &size) override {
        rt.reset(new RenderTarget(size, 1, TextureFormat::RGB8, false, true));
        cam->SetFrameSize(size);
        m_pipeline->Resize(size);
    };

    void OnCursorPos(const dvec2 &pos, const dvec2 &delta) override {
        cam->LookAround(delta, 0.4 * GetTimeDelta(), -1.5, 1.5);
    }

    void OnScroll(const glm::dvec2 &offset) override {
        m_sunRot=clamp((m_sunRot+offset.y*0.1)/M_PI)*M_PI;
    }

    void Start() override {
        rt = make_unique<RenderTarget>(GetResolution(), 1, TextureFormat::RGB8, false, true);
        cam = make_unique<PerspectiveCamera>(GetResolution(), 45, 1, 1000);
        m_pipeline = make_unique<DeferredPBRPipeline>(GetResolution());
        m_terr = make_unique<Terrain>(ivec2(32, 32), 32);
        m_terr->Generate();
        m_terr->SetDrawRange(3);
        cam->SetLocalPos({0, 40, 0});
        m_pipeline->SetFrameCamera(cam.get());
        Enable(Feature::SeamlessCubemap);
    }

    void Update() override {
        vec3 delta;
        if (GetKey(Key::W)) delta += normalize(ProjectOnPlane(cam->Forward(), vec3(0, 1, 0)));
        if (GetKey(Key::A)) delta -= normalize(ProjectOnPlane(cam->Right(), vec3(0, 1, 0)));
        if (GetKey(Key::S)) delta -= normalize(ProjectOnPlane(cam->Forward(), vec3(0, 1, 0)));
        if (GetKey(Key::D)) delta += normalize(ProjectOnPlane(cam->Right(), vec3(0, 1, 0)));
        if (GetKey(Key::Space)) delta += vec3(0, 1, 0);
        if (GetKey(Key::LShift)) delta -= vec3(0, 1, 0);
        delta *= (float) GetTimeDelta() * 20;
        cam->SetLocalPos(cam->GetLocalPos() + delta);

        Enable(Feature::CullFace);
        Enable(Feature::DepthTest);

        ClearColor({0.4f, 0.6f, 0.8f, 1.0f});
        Clear(BufferBit::Color | BufferBit::Depth);

        vec3 sunDir = glm::angleAxis(m_sunRot, normalize(vec3(0, 0.3, 1))) * vec3(1, 0, 0);
        m_pipeline->SetSunDir(sunDir);

        m_pipeline->Bind();
        Viewport({0, 0}, GetResolution());
        ClearColor({0.4f, 0.6f, 0.8f, 1.0f});
        Clear(BufferBit::Color | BufferBit::Depth);
        m_terr->Draw(cam.get());
        m_pipeline->Unbind();

        DrawBasis(cam->GetViewProjMatrix());
        Disable(Feature::CullFace);
        Disable(Feature::DepthTest);
        Disable(Feature::Blend);
        DrawTex((*m_pipeline->GetFinalBuffer())[0]);

        DrawBasis(cam->GetViewProjMatrix());
    }
};

int main(int argc, char *argv[]) {
    cout << std::experimental::filesystem::current_path();
    auto app = make_unique<RunnerApp>();
    app->Run();
    return 0;
}

