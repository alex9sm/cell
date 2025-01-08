#include "scene.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip> 

Scene::Scene(ModelManager* modelManager, UI* ui)
    : m_ModelManager(modelManager)
    , m_UI(ui)
{
}

void Scene::saveState(const std::string& filepath) const {
    try {
        json sceneState = createSceneStateJson();

        std::filesystem::path filePath(filepath);
        if (auto parentPath = filePath.parent_path(); !parentPath.empty()) {
            std::filesystem::create_directories(parentPath);
        }

        std::ofstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + filepath);
        }
        file << std::setw(4) << sceneState << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving scene state: " << e.what() << std::endl;
    }
}

json Scene::createSceneStateJson() const {
    json sceneState;
    json scene;

    scene["version"] = SCENE_VERSION;

    json loadedModels = json::array();
    const auto& modelPaths = m_ModelManager->getLoadedPaths();

    for (const auto& path : modelPaths) {
        json modelInfo;
        std::filesystem::path fullPath(path);
        modelInfo["path"] = fullPath.filename().string();
        loadedModels.push_back(modelInfo);
    }

    scene["models"] = loadedModels;
    sceneState["scene"] = scene;
    return sceneState;
}

void Scene::loadState(const std::string& filepath) {
    try {
        if (!std::filesystem::exists(filepath)) {
            std::cout << "No previous scene state found at: " << filepath << std::endl;
            return;
        }

        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file for reading: " + filepath);
        }

        json sceneState;
        file >> sceneState;

        if (!sceneState.contains("scene") ||
            !sceneState["scene"].contains("version") ||
            !sceneState["scene"].contains("models")) {
            throw std::runtime_error("Invalid scene state file format");
        }

        float version = sceneState["scene"]["version"];

        std::vector<std::string> modelPaths;
        for (const auto& modelInfo : sceneState["scene"]["models"]) {
            modelPaths.push_back(modelInfo["path"]);
        }

        m_ModelManager->updateModelsFromSelection(modelPaths);
        m_UI->updateSelectedModels(modelPaths);
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading scene state: " << e.what() << std::endl;
    }
}