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
#include "camera.h"
#include "skybox/skybox.h"
#include "scene.h"
#include "player_controller.h"
#include "player_collision.h"

int main() {

    Window window(1920, 1080, "Cell");

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

    Shader skyboxShader;
    if (!skyboxShader.init("src/shaders/skybox_vertex.glsl", "src/shaders/skybox_fragment.glsl")) {
        return -1;
    }

    Shader wireframeShader;
    if (!wireframeShader.init("src/shaders/wireframe_vertex.glsl", "src/shaders/wireframe_fragment.glsl")) {
        return -1;
    }

    Skybox skybox;
    if (!skybox.init()) {  // Uses default texture directory
        std::cerr << "Failed to initialize skybox" << std::endl;
        return -1;
    }

    UI ui(window.getHandle());
    if (!ui.init()) {
        return -1;
    }

    ModelManager modelManager;

    Scene scene(&modelManager, &ui);
    ui.setSaveSceneCallback([&scene]() {
        scene.saveState();
        });
    scene.loadState();

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Create view and projection matrices
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),
        static_cast<float>(window.getWidth()) / window.getHeight(),
        0.1f, 800.0f);

    Camera camera(window.getHandle());

    Player player(window.getHandle());
    PlayerCollision playerCollision(modelManager, player);
    PlayerController playerController(camera, player, ui);

    float lastFrame = 0.0f;

    while (!window.shouldClose()) {

        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        camera.update(deltaTime);

        shader.use();
        shader.setInt("diffuseTexture", 0);

        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", projection);

        modelManager.updateModelsFromSelection(ui.getSelectedModels());
        modelManager.renderAll(shader);

        skyboxShader.use();
        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.getViewMatrix()));
        skyboxShader.setMat4("view", skyboxView);
        skyboxShader.setMat4("projection", projection);
        skyboxShader.setInt("skybox", 0); 
        skybox.render(skyboxShader);

        if (!ui.isPlayerMode()) {
            wireframeShader.use();
            wireframeShader.setMat4("view", camera.getViewMatrix());
            wireframeShader.setMat4("projection", projection);
            player.renderAABB(wireframeShader);
        }

        playerController.update(deltaTime);
        playerCollision.update();

        ui.render(camera);

        window.update();
    }

    return 0;
}