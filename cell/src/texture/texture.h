#pragma once
#include <GL/glew.h>
#include <string>

class Texture {
private:
    GLuint m_TextureID;
    int m_Width;
    int m_Height;
    int m_Channels;
    std::string m_Path;

    void setupTexture(unsigned char* data);
    void cleanup();

public:
    Texture();
    ~Texture();

    // Prevent copying of texture objects
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Core functionality
    bool loadTexture(const std::string& path);
    void bind(unsigned int slot = 0) const;
    void unbind() const;

    // Getters
    int getWidth() const { return m_Width; }
    int getHeight() const { return m_Height; }
    GLuint getID() const { return m_TextureID; }
    const std::string& getPath() const { return m_Path; }
};
