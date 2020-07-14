#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Application* Application::m_instance = nullptr;

	bool exitLoader;

	Application::Application(const std::string& caption) : Window(glm::ivec2(800, 800), caption, true) {
		m_startupPath = fs::absolute(fs::current_path());
		m_instance = this;
		MakeContextCurrent();
		m_res = make_unique<ResourceContainer>();
		m_fpsCounter = make_unique<GPUStopwatch<20>>();
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		m_screenSize.x = mode->width;
		m_screenSize.y = mode->height;
		m_lastFrameTimePoint = chrono::high_resolution_clock::now();
		m_startTimePoint = chrono::high_resolution_clock::now();
		CreateInternal();
		m_material = make_unique<Material>();
		GUI::Internal::Init(GetHandle());
		exitLoader = false;
		const int loaderCount = 4;
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		for (int i = 0; i < loaderCount; ++i) {
			GLFWwindow* win = glfwCreateWindow(32, 32, "Loader", nullptr, GetHandle());
			m_loaderWindows.push_back(win);
			m_loaders.emplace_back(Application::LoaderFunc, win);
		}
		m_lines = make_unique<VertexAccumulator>(Topology::Lines);
		m_triangles = make_unique<VertexAccumulator>(Topology::Triangles);
		Particles::Init();
	}

	future<Resource*> Application::LoadAsync(const ResourceProxy* proxy) {
		lock_guard lock(m_instance->m_loadingQueueLock);
		packaged_task<Resource * (void)> task([proxy]() {
			m_instance->m_loadingQueueLock.lock();
			cout << "Thread " << this_thread::get_id() << " working" << endl;
			m_instance->m_loadingQueueLock.unlock();
			auto res = proxy->Build();
			Finish();
			return res;
			});
		future<Resource*> res = task.get_future();
		m_instance->m_loading.push(move(task));
		return res;
	}

	void Application::LoaderFunc(GLFWwindow* window) {
		glfwMakeContextCurrent(window);
		glewInit();
		m_instance->m_loadingQueueLock.lock();
		cout << "Thread " << this_thread::get_id() << " started" << endl;
		m_instance->m_loadingQueueLock.unlock();
		while (!exitLoader) {
			m_instance->m_loadingQueueLock.lock();
			if (m_instance->m_loading.empty()) {
				m_instance->m_loadingQueueLock.unlock();
				this_thread::yield();
			}
			else {
				auto task = move(m_instance->m_loading.front());
				m_instance->m_loading.pop();
				m_instance->m_loadingQueueLock.unlock();
				task();
			}
		}
	}

	dvec2 Application::WorldToScreen(const dvec3& pos)
	{
		if (m_instance->m_debugCamera == nullptr) return dvec2(0, 0);
		dvec4 p = m_instance->m_debugCamera->GetViewProjMatrix() * dvec4(pos, 1);
		p /= p.w;
		p = p * 0.5 + 0.5;
		ivec2 res = m_instance->GetResolution();
		return { p.x * res.x,res.y - p.y * res.y };
	}

	void Application::DrawTex(TextureBase* tex) {
		m_instance->m_drawTex->Bind();
		m_instance->m_drawTex->Set("MVP", dmat4());
		m_instance->m_drawTex->SetTexture("Tex", tex);
		m_instance->m_screenQuad->Draw();
		m_instance->m_drawTex->Unbind();
	}

	void Application::DrawTex(TextureBase* tex, const dvec2& size, const dvec2& pos, double rot)
	{
		ivec2 res = m_instance->GetResolution();
		dmat4 mat = dmat4();
		mat = glm::translate(mat, { -1.0 + 2.0 / res.x * pos.x,-1.0 + 2.0 / res.y * pos.y,0 });
		mat = glm::scale(mat, dvec3(size.x / res.x, size.y / res.y, 1));
		//mat = glm::translate(mat, { 1,1,0 });
		m_instance->m_drawTex->Bind();
		m_instance->m_drawTex->Set("MVP", mat);
		m_instance->m_drawTex->SetTexture("Tex", tex);
		m_instance->m_screenQuad->Draw();
		m_instance->m_drawTex->Unbind();
	}

	void Application::DrawBasis(const dmat4& M) {
		if (m_instance->m_debugCamera == nullptr) return;
		m_instance->m_drawBasis->Bind();
		m_instance->m_drawBasis->Set("MVP", m_instance->m_debugCamera->GetViewProjMatrix() * M);
		m_instance->m_coordBasis->Draw();
		m_instance->m_drawBasis->Unbind();
	}

	void Application::Run() {
		m_res->LoadFromFilesystem();
		m_lastFrameTimePoint = chrono::high_resolution_clock::now();
		m_startTimePoint = chrono::high_resolution_clock::now();
		Start();
		while (!ShouldClose()) {
			ClearColor(glm::vec4(0.6f, 0.8f, 1.f, 0.f));
			Clear(BufferBit::Color | BufferBit::Depth);

			auto ctime = chrono::high_resolution_clock::now();
			m_timeDelta = chrono::duration_cast<chrono::duration<double>>(ctime - m_lastFrameTimePoint).count();
			m_time = chrono::duration_cast<chrono::duration<double>>(ctime - m_startTimePoint).count();
			m_lastFrameTimePoint = ctime;
			m_material->Set<double>("Time", m_time);
			m_material->Set<double>("TimeDelta", m_timeDelta);
			GUI::Internal::NewFrame();
			if (m_debugCamera) {
				m_lines->StartFrame();
				m_triangles->StartFrame();
			}
			m_fpsCounter->Start();
			Update();
			m_fpsCounter->Stop();
			if (m_debugCamera) {
				Clear(BufferBit::Depth);
				Enable(Feature::DepthTest);
				Disable(Feature::Blend);
				m_lines->EndFrame();
				m_triangles->EndFrame();
				m_drawLines->Bind();
				m_drawLines->Set("MVP", m_debugCamera->GetViewProjMatrix());
				m_lines->Draw();
				m_triangles->Draw();
				m_drawLines->Unbind();
			}
			GUI::Internal::Render();
			SwapBuffers();
			PollEvents();
		}
		Shutdown();
		m_res.reset();
	}

	World* Application::CreateWorld(const std::string& name)
	{
		World* w = new World();
		w->SetName(name);
		m_instance->m_worlds.emplace_back(w);
		return w;
	}

	Application::~Application() {
		Particles::Shutdown();
		m_lines.reset();
		m_triangles.reset();
		m_screenQuad.reset();
		m_coordBasis.reset();
		m_drawTex.reset();
		m_drawBasis.reset();
		m_drawLines.reset();
		exitLoader = true;
		for (auto& t : m_loaders)
			if (t.joinable())
				t.join();
		for (auto w : m_loaderWindows)
			glfwDestroyWindow(w);
		GUI::Internal::Shutdown();
		m_fpsCounter.reset();
		for (auto& world : m_worlds)
			world.reset();
		glfwTerminate();
	}

}
