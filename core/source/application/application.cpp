#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {
	Application *Application::m_instance = nullptr;

	bool exitLoader;

	Application::Application(const std::string& caption) : Window(glm::ivec2(800, 600), caption, true) {
		m_instance = this;
		MakeContextCurrent();
		m_res = make_unique<ResourceContainer>();
		m_fpsCounter = make_unique<GPUStopwatch<100>>();
		const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
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
	}

	future<Resource*> Application::LoadAsync(const ResourceProxy* proxy) {
		lock_guard lock(m_instance->m_loadingQueueLock);
		packaged_task<Resource*(void)> task([proxy]() {
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

	Transform* Application::CreateTransform() noexcept {
		Transform* tr = new Transform();
		m_instance->m_roots.emplace_back(tr);
		return tr;
	}

	Actor* Application::CreateActor(const std::string& name) noexcept {
		Transform* tr = new Transform();
		m_instance->m_roots.emplace_back(tr);
		return tr->CreateActor(name);
	}

	void Application::DrawTex(TextureBase *tex) {
		m_instance->m_drawTex->Bind();
		m_instance->m_drawTex->SetTexture("Tex", tex);
		m_instance->m_screenQuad->Draw();
		m_instance->m_drawTex->Unbind();
	}

	void Application::DrawBasis(const dmat4 &MVP) {
		m_instance->m_drawBasis->Bind();
		m_instance->m_drawBasis->Set("MVP", MVP);
		m_instance->m_coordBasis->Draw();
		m_instance->m_drawBasis->Unbind();
	}

	void TransformGUI(const Transform* tr, int seed) {
		const string& name = tr->GetActor() == nullptr ? "transform" : tr->GetActor()->GetName();
		GUI::Tree(name, &tr, seed, [&] {
			int s = 0;
			for (const auto& ch : tr->GetChilds())
				TransformGUI(ch.get(), s++);
		});
	}

	void SceneGUI() {
		GUI::Window("Scene",
			GUI::WindowFlags::Title |
			GUI::WindowFlags::Movable |
			GUI::WindowFlags::Scalable |
			GUI::WindowFlags::Border,
			{ 50,50 }, { 320,480 },
			[&] {
			for (const auto& root : Application::GetRoots())
				TransformGUI(root.get(), 0);
		});
	}

	void Application::Run() {
		m_res->LoadFromFilesystem();
		Start();
		while (!ShouldClose()) {
			ClearColor(glm::vec4(0.6f, 0.8f, 1.f, 0.f));
			Clear(BufferBit::Color | BufferBit::Depth);

			auto ctime = chrono::high_resolution_clock::now();
			m_timeDelta = chrono::duration_cast<chrono::duration<double>>(ctime - m_lastFrameTimePoint).count();
			m_time = chrono::duration_cast<chrono::duration<double>>(ctime - m_startTimePoint).count();
			m_lastFrameTimePoint = ctime;
			m_material->Set<double>("Time", m_time);
			m_material->Set<float>("Time", m_time);
			m_material->Set<double>("TimeDelta", m_timeDelta);
			m_material->Set<float>("TimeDelta", m_timeDelta);
			GUI::Internal::NewFrame();
			m_fpsCounter->Start();
			Update();
			//SceneGUI();
			m_fpsCounter->Stop();
			GUI::Internal::Render();
			SwapBuffers();
			PollEvents();
		}
		Shutdown();
		m_res.reset();
	}

	Application::~Application() {
		exitLoader = true;
		for (auto& t : m_loaders)
			if (t.joinable())
				t.join();
		for (auto w : m_loaderWindows)
			glfwDestroyWindow(w);
		GUI::Internal::Shutdown();
		m_fpsCounter.reset();
		for (auto& root : m_roots)
			root.reset();
		glfwTerminate();
	}

}
