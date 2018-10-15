#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <application/application.h>

#include <other/glstatic.h>
#include <other/enums.h>

using namespace std;
using namespace glm;

namespace ogele {

    Application *Application::m_instance;

    Application::Application() : Window(glm::ivec2(800, 600), "Shader runner", true) {
        m_instance = this;
        MakeContextCurrent();
        m_res = make_unique<ResourceContainer>();
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        m_screenSize.x = mode->width;
        m_screenSize.y = mode->height;
        m_lastFrameTimePoint = chrono::high_resolution_clock::now();
        m_startTimePoint = chrono::high_resolution_clock::now();
        CreateInternal();

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(m_handle, true);
    }

    void Application::DrawTex(TextureBase *tex) {
        m_drawTex->Bind();
        m_drawTex->SetTexture("Tex", tex);
        m_screenQuad->Draw();
        m_drawTex->Unbind();
    }

    void Application::DrawBasis(const dmat4 &MVP) {
        m_drawBasis->Bind();
        m_drawBasis->Set("MVP", MVP);
        m_coordBasis->Draw();
        m_drawBasis->Unbind();
    }

    void Application::Run() {
        m_res->LoadFromFilesystem();
        Start();
        while (!ShouldClose()) {
            ClearColor(glm::vec4(0.6f, 0.8f, 1.f, 0.f));
            Clear(BufferBit::Color | BufferBit::Depth);

            auto ctime = chrono::high_resolution_clock::now();
            m_timeDelta = chrono::duration_cast<chrono::duration<double>>(ctime - m_lastFrameTimePoint).count();
            m_time = chrono::duration_cast<chrono::duration<double>>(ctime-m_startTimePoint).count();
            m_lastFrameTimePoint = ctime;

            ImGui_ImplGlfwGL3_NewFrame();
            Update();
            ImGui::Render();
            SwapBuffers();
            PollEvents();
        }
        Shutdown();
        m_res.reset();
    }

    Application::~Application() {
        ImGui_ImplGlfwGL3_Shutdown();
        glfwTerminate();
    }

}
