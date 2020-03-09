#include <stdafx.h>
#include <ogele.h>

using namespace ogele;

class RunnerApp : public Application {
public:
	RunnerApp() : Application("Sandbox") {};
private:
	struct Triangle {
		vec4 Position[3];
		vec4 Normal[3];
		vec4 TriNormal;
		vec4 NormDot;
		vec4 Domain;
		vec4 Spheres[6];
	};

	Camera* cam;
	res_ptr<ShaderProgram> shader;
	unique_ptr<Material> material;
	res_ptr<ShaderProgram> bakeShader;
	std::vector<res_ptr<Model>> models;
	std::unique_ptr<FloatBuffer<Triangle>> tris;
	int currentModel = 0;
	int triCount = 0;

	void PrepareModel(int index) {
		ModelMesh* mesh = models[index]->GetMesh(0);
		triCount = mesh->GetIndexBuffer()->GetSize() / 3;
		bakeShader->Bind();
		bakeShader->SetBuffer("PosBuffer", mesh->GetPositionBuffer());
		bakeShader->SetBuffer("NormBuffer", mesh->GetNormalsBuffer());
		bakeShader->SetBuffer("IndexBuffer", mesh->GetIndexBuffer());
		bakeShader->SetBuffer("TrisBuffer", tris.get());
		bakeShader->bDispatchCompute({ triCount,1,1 });
		bakeShader->Unbind();
		Barrier(MemoryBarriers::ShaderStorage);
		std::vector<Triangle> tr;
		tr.resize(triCount);
		tris->Bind();
		tris->bGetData(tr.data(), 0, triCount);
		tris->Unbind();
		Log("Baking %s", models[index]->GetName().data());
		LogIndent;
		for (int i = 0; i < triCount; ++i) {
			Log("Triangle %3d", i);
			LogIndent;
			Log("  pos.x(%5g,%5g,%5g),   pos.y(%5g,%5g,%5g),  pos.z(%5g,%5g,%5g)", tr[i].Position[0].x, tr[i].Position[0].y, tr[i].Position[0].z, tr[i].Position[1].x, tr[i].Position[1].y, tr[i].Position[1].z, tr[i].Position[2].x, tr[i].Position[2].y, tr[i].Position[2].z);
			Log(" norm.x(%5g,%5g,%5g),  norm.y(%5g,%5g,%5g), norm.z(%5g,%5g,%5g)", tr[i].Normal[0].x, tr[i].Normal[0].y, tr[i].Normal[0].z, tr[i].Normal[1].x, tr[i].Normal[1].y, tr[i].Normal[1].z, tr[i].Normal[2].x, tr[i].Normal[2].y, tr[i].Normal[2].z);
			Log("trinorm(%5g,%5g,%5g), normdot(%5g,%5g,%5g)", tr[i].TriNormal.x, tr[i].TriNormal.y, tr[i].TriNormal.z, tr[i].NormDot.x, tr[i].NormDot.y, tr[i].NormDot.z);
			Log("domain(%5g,%5g,%5g,%5g)", tr[i].Domain.x, tr[i].Domain.y, tr[i].Domain.z, tr[i].Domain.w);
			LogSpace();
		}
	}

	void OnResize(const ivec2 &size) override {
		cam->SetFrameSize(size);
	};

	void OnCursorPos(const dvec2 &pos, const dvec2 &delta) override {
		if (GetMouseButton(MouseButton::Right) == KeyState::Press)
			cam->LookAround(delta, 0.4 * GetTimeDelta(), -1.5, 1.5);
	}

	void OnScroll(const dvec2 &offset) override {
	}

	void Start() override {
		tris = std::make_unique<FloatBuffer<Triangle>>(BufferFlags::None, 1024);
		shader = Application::GetResourceByName<ShaderProgram>("Raytrace");
		material.reset(shader->CreateMaterial());
		bakeShader = Application::GetResourceByName<ShaderProgram>("Bake");
		cam = Application::CreateWorld("Main")->CreateActor("Camera")->AddComponent<PerspectiveCamera>(GetResolution(), 45.0, 0.1, 10000.0);
		//Application::SetDebugCamera(cam);
		cam->GetTransform()->SetLocalPos({ 4, 4, 10 });
		cam->GetTransform()->LookAt({ 0, 0, 0 });
		models = Application::GetResources<Model>();
		PrepareModel(currentModel);
		Disable(Feature::DepthTest);
		Disable(Feature::CullFace);
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

		GUI::Window("Settings", [this] {
			GUI::Combo("Model", models[currentModel]->GetName(), [this] {
				for (int i = 0; i < models.size(); ++i)
					if (GUI::Button(models[i]->GetName(), { -1, 0 })) {
						currentModel = i;
						PrepareModel(i);
					}
			});
			material->GUI();
		});

		shader->Bind();
		material->Apply(shader.get());
		Application::GetMaterial()->Apply(shader.get());
		shader->Set("IVP", glm::inverse(cam->GetViewProjMatrix()));
		shader->SetBuffer("MeshData", tris.get());
		shader->Set<uint>("TriCount", triCount);
		Application::DrawScreenQuad();
		shader->Unbind();

		DrawBasis(dmat4());
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

