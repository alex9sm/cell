#include "window.h"
#include <iostream>

Window::Window(int width, int height, const std::string& title)
    : m_Window(nullptr)
    , m_Width(width)
    , m_Height(height)
    , m_Title(title)
{
}

Window::~Window() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
    }
    glfwTerminate();
}

bool Window::init() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Make the window's context current
    glfwMakeContextCurrent(m_Window);

    // Set callbacks
    glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallback);

    return true;
}

void Window::update() {
    glfwSwapBuffers(m_Window);
    glfwSwapInterval(0);
    glfwPollEvents();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

void Window::close() {
    glfwSetWindowShouldClose(m_Window, true);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int Window::run() {
    // Initialize window
    if (!init()) {
        return -1;
    }

    // Main loop
    while (!shouldClose()) {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Update window
        update();
    }

    return 0;
}