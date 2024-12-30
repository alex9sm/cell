#pragma once
#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_glfw.h"
#include "../dependencies/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>

class UI {
private:
    GLFWwindow* m_Window;
    bool m_ShowDemoWindow;

    // Initialize ImGui context and implementations
    //void setupImGui();
    // Cleanup ImGui context and implementations
    //void cleanupImGui();

public:
    UI(GLFWwindow* window);
    ~UI();

    // Delete copy constructor and assignment operator
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    // Initialize the UI system
    bool init();
    // Begin new frame and render UI elements
    void render();
    // Cleanup resources
    void cleanup();

    // Toggle demo window
    void toggleDemoWindow() { m_ShowDemoWindow = !m_ShowDemoWindow; }
};