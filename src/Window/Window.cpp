#include "Window.h"

namespace MyWorld {

    GLFWwindow* Window::m_window = nullptr;
    WindowData Window::m_window_data = {};
    bool Window::shouldCloseWindow = false;
    WindowSize Window::size;
    glm::vec2 Window::center;

    void Window::hideCursor()
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    void Window::showCursor()
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Window::setCursorPos(double width, double height)
    {
        glfwSetCursorPos(m_window, width, height);
    }

    GLFWwindow* Window::getGlfwWindowObj()
    {
        return m_window;
    }

    const HWND& Window::getWindowHWND()
    {
        return glfwGetWin32Window(m_window);
    }

    bool Window::shouldWindowTerminate()
    {
        return shouldCloseWindow || glfwWindowShouldClose(m_window);
    }

    void Window::setShouldWindowClose(bool shouldClose)
    {
        shouldCloseWindow = shouldClose;
    }

    void Window::glfw_error_callback(int error, const char* desc)
    {
        fprintf(stderr, "Glfw Error %d: %s\n", error, desc);
    }

    void Window::setEventCallback(const std::function<void(Event& e)>& fn)
    {
        if (!fn) return;

        m_window_data.eventHandler = fn;
    }

    const WindowSize& Window::getWindowSize()
    {
        return size;
    }

    const glm::vec2& Window::getCenter()
    {
        return center;
    }

    void Window::setWindowSize(WindowSize s)
    {
        size = s;
        center = glm::vec2{ s.width / 2, s.height / 2 };
    }

    double Window::getCurrentTime()
    {
        return glfwGetTime();
    }

    void Window::Init(WindowParams &params)
    {
        setWindowSize(params.size);
        // Setup window
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, params.version.major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, params.version.minor);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        // Create window with graphics context
        m_window = glfwCreateWindow(params.size.width, params.size.height, params.title.c_str(), NULL, NULL);
        if (m_window == NULL)
            return;
        //glfwMakeContextCurrent(m_window);
        //glfwSwapInterval(1); // Enable vsync

        glfwSetWindowUserPointer(m_window, &m_window_data);

        // Bind keyboard event callback
        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
            if (!windowData.eventHandler) return;

            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressEvent event(Event::KEY_PRESS, key, KeyPressEvent::KEY_DOWN);
                windowData.eventHandler(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyPressEvent event(Event::KEY_PRESS, key, KeyPressEvent::KEY_UP);
                windowData.eventHandler(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressEvent event(Event::KEY_PRESS, key, KeyPressEvent::KEY_HOLD);
                windowData.eventHandler(event);
                break;
            }
            }
        });

        // Bind mouse button event callback
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
            if (!windowData.eventHandler) return;

            switch (action)
            {
            case GLFW_PRESS:
            {
                MousePressEvent event(Event::MOUSE_PRESS, button, MousePressEvent::MOUSE_DOWN);
                windowData.eventHandler(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MousePressEvent event(Event::MOUSE_PRESS, button, MousePressEvent::MOUSE_UP);
                windowData.eventHandler(event);
                break;
            }
            case GLFW_REPEAT:
            {
                MousePressEvent event(Event::MOUSE_PRESS, button, MousePressEvent::MOUSE_HOLD);
                windowData.eventHandler(event);
                break;
            }
            }
        });

        // Bind mouse scroll event callback
        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
            if (!windowData.eventHandler) return;

            MouseWheelEvent event(Event::MOUSE_WHEEL, (float)xOffset, (float)yOffset);
            windowData.eventHandler(event);
        });

        // Bind mouse cursor position event callback
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& windowData = *(WindowData*)glfwGetWindowUserPointer(window);
            if (!windowData.eventHandler) return;

            MouseMoveEvent event(Event::MOUSE_MOVE, (float)xPos, (float)yPos);
            windowData.eventHandler(event);
        });
    }

    void Window::Terminate()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Window::Begin()
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
    }

    void Window::End()
    {
        // glfwSwapBuffers(m_window);
    }

}
