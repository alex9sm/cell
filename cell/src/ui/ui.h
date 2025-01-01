#pragma once
#include <GL/glew.h>
#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_glfw.h"
#include "../dependencies/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "camera.h"

class UI {
private:
    GLFWwindow* m_Window;
    bool m_ShowDemoWindow;

    std::vector<std::string> m_ModelFiles;        
    std::vector<std::string> m_SelectedModels;   
    int m_CurrentItem;                            

    void refreshModelList();

public:
    UI(GLFWwindow* window);
    ~UI();

    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    bool init();
    void render(Camera& camera);
    void cleanup();

    void toggleDemoWindow() { m_ShowDemoWindow = !m_ShowDemoWindow; }

    const std::vector<std::string>& getSelectedModels() const { return m_SelectedModels; }
};