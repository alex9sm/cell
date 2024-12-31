#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "window/window.h"
#include "shaderfv/shader.h"
#include "ui/ui.h"
#include "model/model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {

    Window window(800, 600, "Cell");

    window.init();
       
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader;
    if (!shader.init("src/shaders/vertex.glsl", "src/shaders/fragment.glsl")) {
        return -1;
    }

    Model model;
    // We'll load a model based on UI selection later, but for testing:
    if (!model.loadModel("gamedata/models/monkey.obj")) {
        std::cerr << "Failed to load model" << std::endl;
        return -1;
    }

    UI ui(window.getHandle());
    if (!ui.init()) {
        return -1;
    }

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Create view and projection matrices
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        static_cast<float>(window.getWidth()) / window.getHeight(),
        0.1f, 100.0f);

    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        shader.use();

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setMat4("model", model.getModelMatrix());

        model.render();

        const std::vector<std::string>& selectedModels = ui.getSelectedModels();
        if (!selectedModels.empty()) {
            // Load the first selected model
            // In a real application, you might want to handle multiple models
            std::string modelPath = "gamedata/models/" + selectedModels[0];
            if (model.loadModel(modelPath)) {
                std::cout << "Successfully loaded model: " << modelPath << std::endl;
            } else {
                std::cerr << "Failed to load model: " << modelPath << std::endl;
            }
        }

        ui.render();

        window.update();
    }

    return 0;
}