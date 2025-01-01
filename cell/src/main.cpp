#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "window/window.h"
#include "shaderfv/shader.h"
#include "ui/ui.h"
#include "model/model_manager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera/camera.h"

int main() {

    Window window(1200, 900, "Cell");

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

    UI ui(window.getHandle());
    if (!ui.init()) {
        return -1;
    }

    ModelManager modelManager;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Create view and projection matrices
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        static_cast<float>(window.getWidth()) / window.getHeight(),
        0.1f, 100.0f);

    Camera camera(window.getHandle());
    float lastFrame = 0.0f;

    while (!window.shouldClose()) {

        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        camera.update(deltaTime);

        shader.use();
        shader.setInt("diffuseTexture", 0);

        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", projection);

        modelManager.updateModelsFromSelection(ui.getSelectedModels());
        modelManager.renderAll(shader);

        ui.render(camera);

        window.update();
    }

    return 0;
}