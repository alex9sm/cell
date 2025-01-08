#pragma once
#include <GL/glew.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <functional>
#include "camera.h"
#include "model_manager.h"
#include "player.h"

class UI {
private:
    GLFWwindow* m_Window;
    bool m_ShowDemoWindow;

    std::vector<std::string> m_ModelFiles;        
    std::vector<std::string> m_SelectedModels;   
    int m_CurrentItem;                            

    void refreshModelList();
    ModelManager* m_ModelManager = nullptr;

    std::function<void()> m_SaveSceneCallback = nullptr;

    bool m_PlayerMode;

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

    void setModelManager(ModelManager* manager) { m_ModelManager = manager; }

    void updateSelectedModels(const std::vector<std::string>& modelNames) {
        m_SelectedModels = modelNames;
    }

    void setSaveSceneCallback(std::function<void()> callback) {
        m_SaveSceneCallback = callback;
    }

    bool isPlayerMode() const { return m_PlayerMode; }
};