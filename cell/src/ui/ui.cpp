#include "ui.h"
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>


UI::UI(GLFWwindow* window)
    : m_Window(window)
    , m_ShowDemoWindow(true)
    , m_CurrentItem(0)
{
    refreshModelList();
}

UI::~UI() {
    cleanup();
}

void UI::refreshModelList() {
    m_ModelFiles.clear();
    const std::filesystem::path modelsPath = "D:\\codingfolder\\CPPprojects\\cell\\cell\\gamedata\\models"; // Change this to your models folder path

    // Check if directory exists
    

    // Gather all 3D model files
    for (const auto& entry : std::filesystem::directory_iterator(modelsPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            // Add relevant 3D model extensions here
            if (extension == ".obj") {
                m_ModelFiles.push_back(entry.path().filename().string());
            }
        }
    }

    // Sort the list alphabetically
    std::sort(m_ModelFiles.begin(), m_ModelFiles.end());
}

bool UI::init() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    if (!ImGui_ImplGlfw_InitForOpenGL(m_Window, true)) {
        std::cerr << "Failed to initialize ImGui GLFW implementation" << std::endl;
        return false;
    }

    const char* glsl_version = "#version 330";
    if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
        std::cerr << "Failed to initialize ImGui OpenGL3 implementation" << std::endl;
        return false;
    }

    return true;
}

void UI::render() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Add your custom windows here
    {
        ImGui::Begin("Cell");
        
        if (ImGui::Button("Refresh Model List")) {
            refreshModelList();
        }

        ImGui::SameLine();

        // Show number of available models
        ImGui::Text("Available Models: %zu", m_ModelFiles.size());

        if (!m_ModelFiles.empty()) {
            if (ImGui::BeginCombo("Select Model", m_ModelFiles[m_CurrentItem].c_str())) {
                for (int i = 0; i < m_ModelFiles.size(); i++) {
                    const bool is_selected = (m_CurrentItem == i);
                    if (ImGui::Selectable(m_ModelFiles[i].c_str(), is_selected)) {
                        m_CurrentItem = i;
                    }

                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            // Add button
            if (ImGui::Button("Add Model")) {
                // Check if model isn't already in the list
                if (std::find(m_SelectedModels.begin(), m_SelectedModels.end(),
                    m_ModelFiles[m_CurrentItem]) == m_SelectedModels.end()) {
                    m_SelectedModels.push_back(m_ModelFiles[m_CurrentItem]);
                }
            }
        }
        else {
            ImGui::Text("No models found in the models directory!");
        }

        // Selected models list
        ImGui::Separator();
        ImGui::Text("Selected Models:");

        // Use a child window for scrolling if the list gets too long
        ImGui::BeginChild("SelectedModels", ImVec2(0, 200), true);
        for (auto it = m_SelectedModels.begin(); it != m_SelectedModels.end();) {
            ImGui::PushID(it->c_str());

            // Display model name
            ImGui::Text("%s", it->c_str());

            ImGui::SameLine();

            // Remove button
            if (ImGui::Button("Remove")) {
                it = m_SelectedModels.erase(it);
            }
            else {
                ++it;
            }

            ImGui::PopID();
        }
        ImGui::EndChild();

        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}