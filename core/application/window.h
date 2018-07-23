//
// Created by ??????? on 18.07.2018.
//

#ifndef OGELE_WINDOW_H
#define OGELE_WINDOW_H
namespace ogele {
    class Window : public HandleBase<GLFWwindow *> {
        Entity(Window)

    private:
        dvec2 m_oldCursorPos;

        static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

        static void CharCallback(GLFWwindow *window, unsigned int character);

        static void MouseBtnCallback(GLFWwindow *window, int button, int action, int mods);

        static void CursorPosCallback(GLFWwindow *window, double x, double y);

        static void CursorEnterCallback(GLFWwindow *window, int entered);

        static void ScrollCallback(GLFWwindow *window, double x, double y);

        static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);

    protected:
        Window(const glm::ivec2 &size, const std::string &caption, bool firstInit);

        virtual void OnKey(Key key, int scancode, KeyState state, KeyModifiers mods) {};

        virtual void OnChar(wchar_t character) {};

        virtual void OnMouseBtn(MouseButton button, KeyState state, KeyModifiers mods) {};

        virtual void OnCursorPos(const glm::dvec2 &pos, const glm::dvec2 &delta) {};

        virtual void OnCursorEnter(bool entered) {};

        virtual void OnScroll(const glm::dvec2 &offset) {};

        virtual void OnResize(const glm::ivec2 &size) {};

        inline void MakeContextCurrent() const { glfwMakeContextCurrent(m_handle); }

        inline void SwapBuffers() { glfwSwapBuffers(m_handle); }

        inline void PollEvents() { glfwPollEvents(); }

        inline bool ShouldClose() const { return glfwWindowShouldClose(m_handle); }

        inline void SetShouldClose(bool value) { glfwSetWindowShouldClose(m_handle, value); }

    public:
        inline bool GetKey(Key key) const { return glfwGetKey(m_handle, static_cast<int>(key)) == GLFW_PRESS; }

        inline KeyState GetMouseButton(MouseButton btn) const {
            return static_cast<KeyState>(glfwGetMouseButton(m_handle, static_cast<int>(btn)));
        }

        inline ivec2 GetResolution() const {
            ivec2 res;
            glfwGetFramebufferSize(m_handle, &res.x, &res.y);
            return res;
        }

        inline dvec2 GetCursorPos() const {
            dvec2 res;
            glfwGetCursorPos(m_handle, &res.x, &res.y);
            return res;
        }

        inline void SetCursorPos(const dvec2 &value) { glfwSetCursorPos(m_handle, value.x, value.y); }

        inline void LockCursor(bool value) const {
            glfwSetInputMode(m_handle, GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }

        ~Window();
    };
}
#endif //OGELE_WINDOW_H
