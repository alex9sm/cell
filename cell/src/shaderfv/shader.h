#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class Shader {
private:
    GLuint m_ProgramID;
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;

    // helper functions
    std::string readShaderFile(const std::string& filePath);
    GLuint compileShader(const std::string& source, GLenum shaderType);
    void checkCompileErrors(GLuint shader, const std::string& type);
    void initBuffers();
    void cleanupBuffers();

public:
    Shader();
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    bool init(const std::string& vertexPath, const std::string& fragmentPath);
    void use();
    void render();
    void cleanup();

    // Utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};
