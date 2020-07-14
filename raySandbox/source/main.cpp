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
		vec4 VertexData[3];
	};

	Camera* cam = nullptr;
	res_ptr<ShaderProgram> shader;
	unique_ptr<Material> material;
	res_ptr<ShaderProgram> bakeShader;
	std::vector<Model*> models;
	std::unique_ptr<FloatBuffer<Triangle>> trisBuffer;
	std::vector<Triangle> tris;
	int currentModel = 0;
	int triCount = 0;
	bool raytarce = false;
	dvec3 nearPos = { -1,1,2 };
	dvec3 farPos = { 1,0,-2 };
	int ctri = -1;

	void PrepareModel(int index) {
		ModelMesh* mesh = models[index]->GetMesh(0);
		triCount = mesh->GetIndexBuffer()->GetSize() / 3;
		bakeShader->Bind();
		bakeShader->SetBuffer("PosBuffer", mesh->GetPositionBuffer());
		bakeShader->SetBuffer("NormBuffer", mesh->GetNormalsBuffer());
		bakeShader->SetBuffer("IndexBuffer", mesh->GetIndexBuffer());
		bakeShader->SetBuffer("TrisBuffer", trisBuffer.get());
		bakeShader->bDispatchCompute({ triCount,1,1 });
		bakeShader->Unbind();
		Barrier(MemoryBarriers::ShaderStorage);
		tris.resize(triCount);
		trisBuffer->Bind();
		trisBuffer->bGetData(tris.data(), 0, triCount);
		trisBuffer->Unbind();
		ctri = -1;
	}

	void OnResize(const ivec2& size) override {
		cam->SetFrameSize(size);
	};

	void OnCursorPos(const dvec2& pos, const dvec2& delta) override {
		if (GetMouseButton(MouseButton::Right) == KeyState::Press)
			cam->LookAround(delta, 0.8 * GetTimeDelta(), -1.5, 1.5);
	}

	void OnScroll(const dvec2& offset) override {
	}

	void OnMouseBtn(MouseButton button, KeyState state, KeyModifiers mods) override {
		if (button == MouseButton::Middle && state == KeyState::Press) {
			dvec2 cpos = GetCursorPos();
			cpos /= GetResolution();
			cpos = cpos * 2.0 - dvec2(1);
			cpos.y *= -1;
			dmat4 ivp = inverse(cam->GetViewProjMatrix());
			dvec4 nPos = ivp * dvec4(cpos.x, cpos.y, -1, 1);
			dvec4 fPos = ivp * dvec4(cpos.x, cpos.y, 1, 1);
			nearPos = nPos / nPos.w;
			farPos = fPos / fPos.w;
		}
	}

	void LoadTri(const string& name, ifstream& file) {
		Log("Loading tri file %s", name.data());
		LogIndent;
		vector<vec4> pos;
		vector<vec4> norm;
		while (!file.eof()) {
			vec4 cpos;
			vec4 cnorm;
			file >> cpos.x >> cpos.y >> cpos.z;
			file >> cnorm.x >> cnorm.y >> cnorm.z;
			cpos *= 0.01;
			Log("pos %f %f %f norm %f %f %f", cpos.x, cpos.y, cpos.z, cnorm.x, cnorm.y, cnorm.z);
			pos.push_back(cpos);
			norm.push_back(cnorm);
		}
		Log("Loaded %d vertices", pos.size());
		ModelMesh* mesh = new ModelMesh({});
		FloatBuffer<vec4>* posBuf = new FloatBuffer<vec4>(BufferFlags::None, pos.size(), pos.data());
		FloatBuffer<vec4>* normBuf = new FloatBuffer<vec4>(BufferFlags::None, norm.size(), norm.data());
		mesh->SetPositionBuffer(posBuf);
		mesh->SetNormalsBuffer(normBuf);
		std::vector<uint> indecies;
		indecies.resize(pos.size());
		for (int i = 0; i < pos.size(); ++i) indecies[i] = i;
		UnsignedIntBuffer<uint>* indeciesBuf = new UnsignedIntBuffer<uint>(BufferFlags::None, indecies.size(), indecies.data());
		mesh->SetIndexBuffer(indeciesBuf);
		Model* mdl = new Model(0);
		mdl->AddMesh(mesh, 0);
		mdl->SetName(name);
		models.push_back(mdl);
	}

	void Start() override {
		auto triFiles = ScanFiles(".tri");
		for (const fs::path& p : triFiles) {
			ifstream istr(p);
			LoadTri(p.filename().string(), istr);
			istr.close();
		}

		trisBuffer = std::make_unique<FloatBuffer<Triangle>>(BufferFlags::None, 1024);
		shader = Application::GetResourceByName<ShaderProgram>("Raytrace");
		material.reset(shader->CreateMaterial());
		material->Set<uint>("Rank", 12);
		bakeShader = Application::GetResourceByName<ShaderProgram>("Bake");
		cam = Application::CreateWorld("Main")->CreateActor("Camera")->AddComponent<PerspectiveCamera>(GetResolution(), 45.0, 0.1, 10000.0);
		Application::SetDebugCamera(cam);
		cam->GetTransform()->SetLocalPos({ 4, 4, 10 });
		cam->GetTransform()->LookAt({ 0, 0, 0 });
		auto resModels = Application::GetResources<Model>();
		for (res_ptr<Model> rm : resModels) models.push_back(rm.get());
		PrepareModel(currentModel);
		Disable(Feature::DepthTest);
		Disable(Feature::CullFace);
	}

	dvec3 Barycentric(dvec3 p, dvec3 a, dvec3 b, dvec3 c)
	{
		double u, v, w;
		dvec3 v0 = b - a, v1 = c - a, v2 = p - a;
		double d00 = dot(v0, v0);
		double d01 = dot(v0, v1);
		double d11 = dot(v1, v1);
		double d20 = dot(v2, v0);
		double d21 = dot(v2, v1);
		double idenom = 1 / (d00 * d11 - d01 * d01);
		v = (d11 * d20 - d01 * d21) * idenom;
		w = (d00 * d21 - d01 * d20) * idenom;
		u = 1.0f - v - w;
		return dvec3(u, v, w);
	}

	double min3(dvec3 v) { return glm::min(glm::min(v.x, v.y), v.z); }
	double max3(dvec3 v) { return glm::max(glm::max(v.x, v.y), v.z); }

	dvec3 Project(dvec3 p, dvec3 o, dvec3 n) {
		dvec3 t = p - o;
		dvec3 d = dot(t, n) * n;
		return p - d;
	}

	double Height(dvec3 p, dvec3 o, dvec3 n) {
		return dot(p - o, n);
	}

	double Dist(dvec3 p, dvec3 d, dvec3 o, dvec3 n) {
		double h = Height(p, o, n);
		return h / dot(d, n);
	}

	double SphereDist(dvec3 pos, dvec3 dir, dvec4 sphere) {
		dvec2 r = rsi(pos - sphere.xyz(), dir, abs(sphere.w));
		return r.x - sign(sphere.w) * r.y;
	}

	double PlaneDist(dvec3 pos, dvec3 dir, dvec3 planePos, dvec3 planeNorm) {
		return dot(pos - planePos, planeNorm) / dot(planeNorm, -dir);
	}

	dvec4 SplineFieldBezier(Triangle tri, dvec3 pos, bool drawPlane) {
		array<dvec3, 3> ppos;
		dvec3 weights;
		double height = Height(pos, tri.Position[0].xyz(), tri.TriNormal.xyz());
		for (int i = 0; i < 3; i++) ppos[i] = dvec3(tri.Position[i].xyz()) + dvec3(tri.Normal[i].xyz()) / (double)tri.NormDot[i] * height;
		weights = Barycentric(pos, ppos[0], ppos[1], ppos[2]);
		if (height < 0) return dvec4(weights, height);
		dvec3 triPos = dvec3(tri.Position[0].xyz()) * weights.x + dvec3(tri.Position[1].xyz()) * weights.y + dvec3(tri.Position[2].xyz()) * weights.z;
		dvec3 dir = normalize(pos - triPos);
		//dvec3 dir = normalize(triPos - pos);
		for (int i = 0; i < 3; i++) ppos[i] = Project(triPos, tri.Position[i].xyz(), tri.Normal[i].xyz());
		dvec3 phPos = ppos[0] * weights[0] + ppos[1] * weights[1] + ppos[2] * weights[2];
		DrawLine({ triPos, phPos }, { 1,1,1 });
		double phHeight = dot(dir, phPos - triPos) * 0.7;
		if (drawPlane) {
			for (int i = 0; i < 3; i++) DrawLine({ tri.Position[i].xyz(), ppos[i] }, { 1,1,1 });
			for (int i = 0; i < 3; i++) DrawLine({ ppos[i], ppos[(i + 1) % 3] }, { 1,1,1 });
			GUI::Text("Tri coords %f, %f, %f", weights.x, weights.y, weights.z);
			for (int i = 0; i < 3; i++) GUI::Text("Normal%d %f", i, length(tri.Normal[i].xyz()));
		}
		//return dvec4(weights, PlaneDist(pos,dir, phPos, -dir));
		return dvec4(weights, distance(pos, triPos) - phHeight);
	}

	double AngleBetween(dvec3 a, dvec3 b) { return glm::acos(dot(normalize(a), normalize(b))); }

	dvec4 CalculateSphere(dvec3 a, dvec3 b, dvec3 n) {
		dvec3 d = (b - a) / 2.0;
		double r = length(d) / dot(-n, normalize(d));
		return dvec4(a - n * r, abs(r));
	}

	dvec4 BlendSpheres(dvec3 pos, dvec3 norm, double c0, double c1, dvec2 w) {
		double c = c0 * w.x + c1 * w.y;
		c = c > 0 ? glm::max(c, 1e-6) : glm::min(c, -1e-6);
		double r = 1 / c;
		return dvec4(pos - norm * r, r);
	}

	dvec4 SplineFieldSpheres(Triangle tri, dvec3 pos, bool selected) {
		dvec3 ppos[3];
		dvec3 weights;
		double height = Height(pos, tri.Position[0].xyz(), tri.TriNormal.xyz());
		for (int i = 0; i < 3; i++) ppos[i] = dvec3(tri.Position[i].xyz()) + dvec3(tri.Normal[i].xyz()) / (double)tri.NormDot[i] * height;
		weights = Barycentric(pos, ppos[0], ppos[1], ppos[2]);
		dvec3 cw = glm::clamp(weights, dvec3(0), dvec3(1));
		cw /= cw.x + cw.y + cw.z;
		//dvec3 triPos = dvec3(tri.Position[0].xyz()) * weights.x + dvec3(tri.Position[1].xyz()) * weights.y + dvec3(tri.Position[2].xyz()) * weights.z;
		dvec3 triPos = dvec3(tri.Position[0].xyz()) * cw.x + dvec3(tri.Position[1].xyz()) * cw.y + dvec3(tri.Position[2].xyz()) * cw.z;
		if (selected) {
			//DrawLine({ ppos[0],ppos[1],ppos[2],ppos[0] }, { 0.5,0.5,0.5 });
			DrawBox(triPos, 0.006, { 0.0,0.0,0.0 });
		}
		dvec3 dir = normalize(triPos - pos) * sign(height);
		dvec2 w01 = weights.xy() / (weights.x + weights.y);
		dvec2 w12 = weights.yz() / (weights.y + weights.z);
		dvec2 w02 = weights.xz() / (weights.x + weights.z);

		w01.x = glm::clamp(AngleBetween(triPos - dvec3(tri.Position[2].xyz()), (tri.Position[1] - tri.Position[2]).xyz()) / tri.VertexData[2].z, 0.0, 1.0);
		w12.x = glm::clamp(AngleBetween(triPos - dvec3(tri.Position[0].xyz()), (tri.Position[2] - tri.Position[0]).xyz()) / tri.VertexData[0].z, 0.0, 1.0);
		w02.x = glm::clamp(AngleBetween(triPos - dvec3(tri.Position[1].xyz()), (tri.Position[2] - tri.Position[1]).xyz()) / tri.VertexData[1].z, 0.0, 1.0);
		w01.y = 1 - w01.x;
		w12.y = 1 - w12.x;
		w02.y = 1 - w02.x;

		array<dvec4, 3> s;
		s[0] = BlendSpheres(tri.Position[0].xyz(), tri.Normal[0].xyz(), tri.VertexData[0].x, tri.VertexData[0].y, w12);
		s[1] = BlendSpheres(tri.Position[1].xyz(), tri.Normal[1].xyz(), tri.VertexData[1].x, tri.VertexData[1].y, w02);
		s[2] = BlendSpheres(tri.Position[2].xyz(), tri.Normal[2].xyz(), tri.VertexData[2].x, tri.VertexData[2].y, w01);

		dvec3 dist;
		dist.x = SphereDist(pos, dir, s[0]);
		dist.y = SphereDist(pos, dir, s[1]);
		dist.z = SphereDist(pos, dir, s[2]);
		if (selected) {
			GUI::TextColored({ 1,0,0,1 }, "%f %f %f", 1.0 / tri.VertexData[0].x, s[0].w, 1.0 / tri.VertexData[0].y);
			GUI::TextColored({ 0,1,0,1 }, "%f %f %f", 1.0 / tri.VertexData[1].x, s[1].w, 1.0 / tri.VertexData[1].y);
			GUI::TextColored({ 0,0,1,1 }, "%f %f %f", 1.0 / tri.VertexData[2].x, s[2].w, 1.0 / tri.VertexData[2].y);
			for (int i = 0; i < 3; ++i) {
				dvec3 vnorm = dvec3(tri.Normal[i].xyz());
				vec3 circleColor(0);
				circleColor[i] = 1;
				//DrawCircle(s[i].xyz(), vnorm * s[i].w, pos - s[i].xyz(), 128, circleColor);
				dvec3 hitPos = pos + dir * dist[i];
				DrawBox(hitPos, 0.004, circleColor);
				DrawCircleSegment(s[i].xyz(), s[i].w, tri.Position[i].xyz(), hitPos, 64, circleColor);
				//DrawLine({ tri.Position[i].xyz(), s[i].xyz() }, circleColor);
			}
		}
		double d = dot(dist, weights);
		dvec3 dpos = pos + dir * d;
		DrawLine({ pos, dpos }, { 0,1,0 });
		DrawLine({ dpos, triPos }, { 1,0,0 });
		return dvec4(weights, d);
	}

	dvec2 rsi(dvec3 r0, dvec3 rd, float sr) {
		double a = dot(rd, rd);
		double b = 2.0 * dot(rd, r0);
		double c = dot(r0, r0) - (sr * sr);
		double d = (b * b) - 4.0 * a * c;
		if (d < 0.0) return dvec2(1e5, -1e5);
		return dvec2(-b / (2.0 * a), sqrt(d) / (2.0 * a));
	}

	void DrawBox(const dvec3& pos, double size, const vec3& color) {
		double h = size / 2.0;
		DrawTriangle({ pos + dvec3(-h,h,h), pos + dvec3(h,h,h), pos + dvec3(h,-h,h) }, color);
		DrawTriangle({ pos + dvec3(-h,h,h), pos + dvec3(-h,-h,h), pos + dvec3(h,-h,h) }, color);
		DrawTriangle({ pos + dvec3(-h,h,h), pos + dvec3(h,h,h), pos + dvec3(h,h,-h) }, color);
		DrawTriangle({ pos + dvec3(-h,h,h), pos + dvec3(-h,h,-h), pos + dvec3(h,h,-h) }, color);
		DrawTriangle({ pos + dvec3(h,-h,h), pos + dvec3(h,h,h), pos + dvec3(h,h,-h) }, color);
		DrawTriangle({ pos + dvec3(h,-h,h), pos + dvec3(h,-h,-h), pos + dvec3(h,h,-h) }, color);

		DrawTriangle({ pos + dvec3(-h,h,-h), pos + dvec3(h,h,-h), pos + dvec3(h,-h,-h) }, color);
		DrawTriangle({ pos + dvec3(-h,h,-h), pos + dvec3(-h,-h,-h), pos + dvec3(h,-h,-h) }, color);
		DrawTriangle({ pos + dvec3(-h,-h,h), pos + dvec3(h,-h,h), pos + dvec3(h,-h,-h) }, color);
		DrawTriangle({ pos + dvec3(-h,-h,h), pos + dvec3(-h,-h,-h), pos + dvec3(h,-h,-h) }, color);
		DrawTriangle({ pos + dvec3(-h,-h,h), pos + dvec3(-h,h,h), pos + dvec3(-h,h,-h) }, color);
		DrawTriangle({ pos + dvec3(-h,-h,h), pos + dvec3(-h,-h,-h), pos + dvec3(-h,h,-h) }, color);
	}

	void DrawCircleSegment(const dvec3& c, double radius, const dvec3& a, const dvec3& b, int segments, const vec3& color) {
		vector<dvec3> p;
		for (int i = 0; i <= segments; ++i) {
			dvec3 t = mix(a, b, ((float)i) / segments);
			t = normalize(t - c) * radius + c;
			p.push_back(t);
		}
		DrawLine(p, color);
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
			GUI::Checkbox("Raytrace", &raytarce);
			GUI::Text("Triangle count %d", tris.size());
			GUI::Text("Render time %5.0f ms", GetFrameTime() * 1000);
			GUI::Combo("Model", models[currentModel]->GetName(), [this] {
				for (int i = 0; i < models.size(); ++i)
					if (GUI::Button(models[i]->GetName(), { -1, 0 })) {
						currentModel = i;
						PrepareModel(i);
					}
				});
			GUI::Input("Selected triangle", ctri);
			ctri = std::min(ctri, (int)tris.size() - 1);
			ctri = std::max(ctri, -1);
			material->GUI();
			});

		if (raytarce) {
			shader->Bind();
			material->Apply(shader.get());
			Application::GetMaterial()->Apply(shader.get());
			shader->Set("IVP", glm::inverse(cam->GetViewProjMatrix()));
			shader->SetBuffer("MeshData", trisBuffer.get());
			shader->Set<uint>("TriCount", triCount);
			Application::DrawScreenQuad();
			shader->Unbind();
			return;
		}
		//DrawLine({ nearPos, farPos }, { 1,1,1 });
		dvec3 view = normalize(farPos - nearPos);
		int Rank = material->Get<uint>("Rank");
		int density = int(pow(2, Rank));
		double minDelta = 0.1;

		for (int j = 0; j < tris.size(); ++j) {
			bool ctriSel = ctri == j || ctri == -1;
			if (!ctriSel) continue;
			const Triangle& tri = tris[j];
			array<dvec3, 3> pos = { tri.Position[0].xyz(),tri.Position[1].xyz(),tri.Position[2].xyz() };
			array<dvec3, 3> norm = { tri.Normal[0].xyz(),tri.Normal[1].xyz(),tri.Normal[2].xyz() };

			double offset = 0.001;
			DrawTriangle(pos, { 0.6,0.6,0.6 });
			dvec3 triCen = (pos[0] + pos[1] + pos[2]) / 3.0;
			if (ctri == -1) GUI::Place::Text(WorldToScreen(triCen), "%d", j);
			DrawLine({ pos[0] + norm[0] * offset, pos[1] + norm[1] * offset, pos[2] + norm[2] * offset, pos[0] + norm[0] * offset }, { 1.0,1.0,1.0 });

			/*dvec4 s1, s2;
			s1.w = 1.0 / tri.VertexData[0].x;
			s1.xyz = dvec3(tri.Position[0].xyz()) - dvec3(tri.Normal[0].xyz()) * s1.w;
			DrawCircleSegment(s1.xyz, s1.w, tri.Position[0].xyz(), tri.Position[1].xyz(), 64, { 0,0,0 });
			s2.w = 1.0 / tri.VertexData[0].y;
			s2.xyz = dvec3(tri.Position[0].xyz()) - dvec3(tri.Normal[0].xyz()) * s2.w;
			DrawCircleSegment(s2.xyz, s2.w, tri.Position[0].xyz(), tri.Position[2].xyz(), 64, { 0,0,0 });*/

			dvec2 bounds = rsi(nearPos - dvec3(tri.Domain.xyz()), view, tri.Domain.w);
			if (bounds.y < 0 || bounds.x - bounds.y < 0) continue;
			dvec3 near = nearPos + view * (bounds.x - bounds.y);
			dvec3 far = nearPos + view * (bounds.x + bounds.y);
			DrawLine({ near, far }, { 1,1,0 });
			/*vec3 near=nearPos;
			vec3 far=farPos;*/
			double totalLength = length(far - near);
			double deltaLength = glm::max(totalLength / density, minDelta);
			dvec3 curDelta = (far - near) / 2.0;
			dvec3 cpos = (near + far) / 2.0;
			bool hit = false;
			double curScene = 1000.0;
			dvec3 curNorm = vec3(0);
			dvec3 weights = vec3(0);
			double dir = 0;
			for (int i = 1; i < Rank; i++)
			{
				bool selected = distance(WorldToScreen(cpos), GetCursorPos()) < 4;
				dvec4 t = SplineFieldSpheres(tri, cpos, selected);
				curScene = t.a;
				weights = t.xyz();
				//curNorm=tri.Normal[0].xyz*weights.r+tri.Normal[1].xyz*weights.g+tri.Normal[2].xyz*weights.b;
				curDelta /= 2;
				dir = sign(curScene);
				//dir=min(sign(dot(curNorm,view)),dir);
				if (dir == 0) dir = 1;
				cpos += dir * curDelta;
			}
			hit = abs(curScene) <= deltaLength;
			if (hit && ctriSel) DrawBox(cpos, 0.006, { 1,1,0 });
		}
	}
};

int main(int argc, char* argv[]) {
	try {
		auto app = std::make_unique<RunnerApp>();
		app->Run();
	}
	catch (const ShaderCompileException & ex) {
		std::cout << ex.what();
		std::cin.get();
	}
	catch (const GLError & ex) {
		std::cout << ex.what();
		std::cin.get();
	}
}

