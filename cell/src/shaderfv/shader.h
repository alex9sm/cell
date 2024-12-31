#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
private:
    GLuint m_ProgramID;

    // Helper functions for shader compilation and error checking
    std::string readShaderFile(const std::string& filePath);
    GLuint compileShader(const std::string& source, GLenum shaderType);
    void checkCompileErrors(GLuint shader, const std::string& type);

public:
    Shader();
    ~Shader();

    // Prevent copying since we're managing OpenGL resources
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    bool init(const std::string& vertexPath, const std::string& fragmentPath);
    void use();
    void cleanup();

    // Uniform setters
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
};
