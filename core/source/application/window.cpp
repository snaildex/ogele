#include <stdafx.h>
#include <ogele.h>
using namespace std;
using namespace glm;
namespace ogele {

    void Window::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnKey(static_cast<Key>(key), scancode, static_cast<KeyState>(action), static_cast<KeyModifiers>(mods));
    }

    void Window::CharCallback(GLFWwindow *window, unsigned int character) {
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnChar(static_cast<wchar_t>(character));
    }

    void Window::MouseBtnCallback(GLFWwindow *window, int button, int action, int mods) {
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnMouseBtn(static_cast<MouseButton>(button), static_cast<KeyState>(action),
                        static_cast<KeyModifiers>(mods));
    }

    void Window::CursorPosCallback(GLFWwindow *window, double x, double y) {
        dvec2 pos = dvec2(x, y);
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnCursorPos(pos, pos - wnd->m_oldCursorPos);
        wnd->m_oldCursorPos = pos;
    }

    void Window::CursorEnterCallback(GLFWwindow *window, int entered) {
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnCursorEnter(static_cast<bool>(entered));
    }

    void Window::ScrollCallback(GLFWwindow *window, double x, double y) {
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        wnd->OnScroll(glm::dvec2(x, y));
    }

    void Window::FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
        if(width==0 || height==0)
            return;
        Window *wnd = (Window *) glfwGetWindowUserPointer(window);
        Viewport(ivec2(0, 0), ivec2(width, height));
        wnd->OnResize(ivec2(width, height));
    }

    Window::Window(const glm::ivec2 &size, const std::string &caption, bool firstInit) {
        if (firstInit)
            glfwInit();
        m_handle = glfwCreateWindow(size.x, size.y, caption.data(), NULL, NULL);
        glfwSetWindowUserPointer(m_handle, this);
        glfwSetWindowSizeLimits(m_handle, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);

        if (firstInit) {
            MakeContextCurrent();
            glewInit();
        }
        glfwSetKeyCallback(m_handle, KeyCallback);
        glfwSetCharCallback(m_handle, CharCallback);
        glfwSetMouseButtonCallback(m_handle, MouseBtnCallback);
        glfwSetCursorPosCallback(m_handle, CursorPosCallback);
        glfwSetCursorEnterCallback(m_handle, CursorEnterCallback);
        glfwSetFramebufferSizeCallback(m_handle, FramebufferSizeCallback);
        glfwSetScrollCallback(m_handle, ScrollCallback);
        glfwGetCursorPos(m_handle, &m_oldCursorPos.x, &m_oldCursorPos.y);
    }

    Window::~Window() {
        glfwDestroyWindow(m_handle);
    }

}