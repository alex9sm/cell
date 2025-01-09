#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "tinyobj/tiny_obj_loader.h"
#include "material.h"

class Model {
private:
    // OpenGL buffer objects
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;

    // Mesh data
    std::vector<float> m_Vertices;  // Positions, normals, and texture coordinates interleaved
    std::vector<unsigned int> m_Indices;

    std::vector<std::shared_ptr<Material>> m_Materials;

    // Model properties 
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

    // Helper functions
    void setupMesh();
    void processModelData(const tinyobj::attrib_t& attrib,
        const std::vector<tinyobj::shape_t>& shapes,
        const std::vector<tinyobj::material_t>& materials);
    bool loadMaterialTextures(const std::vector<tinyobj::material_t>& materials,
        const std::string& modelPath);

public:
    Model();
    ~Model();

    // Delete copy constructor and assignment operator
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    // Core functionality
    bool loadModel(const std::string& filepath);
    void render();
    void cleanup();

    // Transformations
    void setPosition(const glm::vec3& position) { m_Position = position; }
    void setRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
    void setScale(const glm::vec3& scale) { m_Scale = scale; }

    glm::mat4 getModelMatrix() const;

    std::vector<int> m_MaterialIndices;

    const std::vector<float>& getVertices() const { return m_Vertices; }
    const std::vector<unsigned int>& getIndices() const { return m_Indices; }
};