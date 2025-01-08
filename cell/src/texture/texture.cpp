#include "texture.h"
#include <stbimage/stb_image.h>
#include <iostream>
#include <filesystem>

Texture::Texture()
    : m_TextureID(0)
    , m_Width(0)
    , m_Height(0)
    , m_Channels(0)
    , m_Path("")
{
}

Texture::~Texture() {
    cleanup();
}

bool Texture::loadTexture(const std::string& path) {
    m_Path = path;

    std::cout << "Attempting to load texture from: " << path << std::endl;

    // Check if file exists
    if (!std::filesystem::exists(path)) {
        std::cerr << "Texture file does not exist at path: " << path << std::endl;
        return false;
    }

    // Load image using stb_image
    stbi_set_flip_vertically_on_load(true);  
    unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        return false;
    }

    // Setup texture in OpenGL
    setupTexture(data);

    // Free the image data
    stbi_image_free(data);

    return true;
}

void Texture::setupTexture(unsigned char* data) {
    // Generate texture
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture data
    GLenum format;
    if (m_Channels == 1)
        format = GL_RED;
    else if (m_Channels == 3)
        format = GL_RGB;
    else if (m_Channels == 4)
        format = GL_RGBA;
    else {
        std::cerr << "Unsupported number of channels: " << m_Channels << std::endl;
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::cleanup() {
    if (m_TextureID != 0) {
        glDeleteTextures(1, &m_TextureID);
        m_TextureID = 0;
    }
}