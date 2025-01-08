#pragma once
#include <json/json.hpp>
#include <string>
#include <vector>
#include "model_manager.h"
#include "ui.h"

using json = nlohmann::json;

class Scene {
private:
    ModelManager* m_ModelManager;
    UI* m_UI;

    json createSceneStateJson() const;
    static constexpr float SCENE_VERSION = 1.0f;

public:
    Scene(ModelManager* modelManager, UI* ui);

    void saveState(const std::string& filepath = "appdata/scene_state.json") const;
    void loadState(const std::string& filepath = "appdata/scene_state.json");

    void setModelManager(ModelManager* manager) { m_ModelManager = manager; }
    void setUI(UI* ui) { m_UI = ui; }
};