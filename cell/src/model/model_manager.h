#pragma once
#include "model.h"
#include <vector>
#include <memory>
#include <string>
#include "shader.h"

class ModelManager {
private:
    // Store models using smart pointers for automatic memory management
    std::vector<std::unique_ptr<Model>> m_LoadedModels;

    // Keep track of loaded model paths to prevent duplicates
    std::vector<std::string> m_LoadedPaths;

public:
    ModelManager() = default;
    ~ModelManager() = default;

    // Prevent copying since we're managing unique resources
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    // Core functionality
    void updateModelsFromSelection(const std::vector<std::string>& selectedModels);
    void renderAll(const Shader& shader);
    void cleanup();

    // Helper methods
    bool isModelLoaded(const std::string& modelPath) const;
    void removeUnselectedModels(const std::vector<std::string>& selectedModels);
};