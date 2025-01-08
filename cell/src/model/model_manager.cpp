#include "model_manager.h"
#include <algorithm>
#include <iostream>
#include <filesystem>

void ModelManager::updateModelsFromSelection(const std::vector<std::string>& selectedModels) {
    // First, remove any models that are no longer selected
    removeUnselectedModels(selectedModels);

    // Then, load any newly selected models
    for (const auto& modelPath : selectedModels) {
        // Create the full path
        std::string fullPath = "gamedata/models/" + modelPath;

        // Check if this model is already loaded
        if (!isModelLoaded(fullPath)) {
            // Create a new model
            auto newModel = std::make_unique<Model>();

            // Try to load the model
            if (newModel->loadModel(fullPath)) {
                std::cout << "Successfully loaded model: " << modelPath << std::endl;

            
                // Store the model and its path
                m_LoadedModels.push_back(std::move(newModel));
                m_LoadedPaths.push_back(fullPath);
            }
            else {
                std::cerr << "Failed to load model: " << modelPath << std::endl;
            }
        }
    }
}

void ModelManager::renderAll(const Shader& shader) {
    // Render all loaded models
    for (const auto& model : m_LoadedModels) {
        // Update the model matrix uniform for this specific model
        shader.setMat4("model", model->getModelMatrix());
        model->render();
    }
}

void ModelManager::cleanup() {
    m_LoadedModels.clear();
    m_LoadedPaths.clear();
}

bool ModelManager::isModelLoaded(const std::string& modelPath) const {
    return std::find(m_LoadedPaths.begin(), m_LoadedPaths.end(), modelPath) != m_LoadedPaths.end();
}

void ModelManager::removeUnselectedModels(const std::vector<std::string>& selectedModels) {
    std::vector<std::string> selectedFullPaths;
    // Create full paths for comparison
    for (const auto& model : selectedModels) {
        selectedFullPaths.push_back("gamedata/models/" + model);
    }

    // Remove models that are no longer in the selection
    for (size_t i = 0; i < m_LoadedPaths.size();) {
        if (std::find(selectedFullPaths.begin(), selectedFullPaths.end(), m_LoadedPaths[i])
            == selectedFullPaths.end()) {
            // Model is no longer selected, remove it
            m_LoadedModels.erase(m_LoadedModels.begin() + i);
            m_LoadedPaths.erase(m_LoadedPaths.begin() + i);
        }
        else {
            ++i;
        }
    }
}

