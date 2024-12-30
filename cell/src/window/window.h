#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
private:
    GLFWwindow* m_Window;
    int m_Width;
    int m_Height;
    std::string m_Title;

    // Callback setup helper
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

public:
    Window(int width, int height, const std::string& title);
    ~Window();

    // Delete copy constructor and assignment operator
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Main run function
    int run();

    // Getters
    GLFWwindow* getHandle() const { return m_Window; }
    int getWidth() const { return m_Width; }
    int getHeight() const { return m_Height; }

    // Setters
    void setWidth(int width) { m_Width = width; }
    void setHeight(int height) { m_Height = height; }

    // Core window functions
    bool init();
    void update();
    bool shouldClose() const;
    void close();

};