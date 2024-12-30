#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "window/window.h"
#include "shaderfv/shader.h"
#include "ui/ui.h"

//hshshshshs
int main() {

    Window window(1920, 1080, "Cell");

    window.init();
       
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    Shader shader;
    if (!shader.init("src/shaders/vertex.glsl", "src/shaders/fragment.glsl")) {
        return -1;
    }

    UI ui(window.getHandle());
    if (!ui.init()) {
        return -1;
    }

    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);

        shader.render();

        ui.render();

        window.update();
    }

    return 0;
}