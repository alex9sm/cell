#include "ui.h"
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include "camera.h"

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

    // gather all 3d model files
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
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

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

//ui definitions and render
void UI::render(Camera& camera) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Cell");

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate,
            ImGui::GetIO().Framerate);

        ImGui::Separator();

        ImGui::Text("Camera Controls");

        static float speed = camera.m_MovementSpeed;
        if (ImGui::SliderFloat("Camera Speed", &speed, 1.0f, 50.0f, "%.1f deg")) {
            camera.m_MovementSpeed = speed;
        }

        ImGui::Separator();
        
        if (ImGui::Button("Refresh Model List")) {
            refreshModelList();
        }

        ImGui::SameLine();

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

            if (ImGui::Button("Add Model")) {
                if (std::find(m_SelectedModels.begin(), m_SelectedModels.end(),
                    m_ModelFiles[m_CurrentItem]) == m_SelectedModels.end()) {
                    m_SelectedModels.push_back(m_ModelFiles[m_CurrentItem]);
                }
            }
        }
        else {
            ImGui::Text("No models found in the models directory!");
        }

        ImGui::Separator();
        ImGui::Text("Selected Models:");

        ImGui::BeginChild("SelectedModels", ImVec2(0, 200), true);
        for (auto it = m_SelectedModels.begin(); it != m_SelectedModels.end();) {
            ImGui::PushID(it->c_str());
            //display model name
            ImGui::Text("%s", it->c_str());

            ImGui::SameLine();

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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}