#include "skybox.h"
#include <iostream>
#include <filesystem>
#include "stbimage/stb_image.h"

namespace {
    // Skybox vertex positions
    const float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
    };

    // Skybox indices
    const unsigned int skyboxIndices[] = {
        // Right
        3, 2, 6,
        6, 7, 3,
        // Left
        0, 4, 5,
        5, 1, 0,
        // Top
        4, 0, 3,
        3, 7, 4,
        // Bottom
        1, 5, 6,
        6, 2, 1,
        // Back
        4, 7, 6,
        6, 5, 4,
        // Front
        0, 1, 2,
        2, 3, 0,
    };
}

Skybox::Skybox()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
    , m_CubemapTexture(0)
    , m_TextureDirectory("")
{
}

Skybox::~Skybox() {
    cleanup();
}

std::array<std::string, 6> Skybox::getDefaultTexturePaths() const {
    return {
        m_TextureDirectory + "/nx.png",
        m_TextureDirectory + "/nz.png",
        m_TextureDirectory + "/py.png",
        m_TextureDirectory + "/ny.png",
        m_TextureDirectory + "/px.png",
        m_TextureDirectory + "/px.png"
    };
}

bool Skybox::init(const std::string& textureDir) {
    // Verify texture directory exists
    if (!std::filesystem::exists(textureDir)) {
        std::cerr << "Skybox texture directory does not exist: " << textureDir << std::endl;
        return false;
    }

    m_TextureDirectory = textureDir;

    // Check if all required textures exist
    auto texturePaths = getDefaultTexturePaths();
    for (const auto& path : texturePaths) {
        if (!std::filesystem::exists(path)) {
            std::cerr << "Missing skybox texture: " << path << std::endl;
            return false;
        }
    }

    setupMesh();
    return loadCubemap();
}

void Skybox::setupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

bool Skybox::loadCubemap() {
    auto faces = getDefaultTexturePaths();

    glGenTextures(1, &m_CubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);

    stbi_set_flip_vertically_on_load(false); // Important for cubemaps

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
            return false;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return true;
}

void Skybox::render(const Shader& shader) const {
    // Change depth function so depth test passes when values are equal to depth buffer's content
    glDepthFunc(GL_LEQUAL);

    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    // Set depth function back to default
    glDepthFunc(GL_LESS);
}

void Skybox::cleanup() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if (m_EBO != 0) {
        glDeleteBuffers(1, &m_EBO);
        m_EBO = 0;
    }
    if (m_CubemapTexture != 0) {
        glDeleteTextures(1, &m_CubemapTexture);
        m_CubemapTexture = 0;
    }
}