#pragma once
#include <GL/glew.h>
#include <string>
#include <array>
#include <vector>
#include "shader.h"

class Skybox {
private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    GLuint m_CubemapTexture;
    std::string m_TextureDirectory;

    void setupMesh();
    bool loadCubemap();
    void cleanup();
    std::array<std::string, 6> getDefaultTexturePaths() const;

public:
    Skybox();
    ~Skybox();

    // Prevent copying
    Skybox(const Skybox&) = delete;
    Skybox& operator=(const Skybox&) = delete;

    // Initialize the skybox
    bool init(const std::string& textureDir = "gamedata/textures/skybox_cubemap");

    // Render the skybox
    void render(const Shader& shader) const;
};