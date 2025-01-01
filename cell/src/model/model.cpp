#include "model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Model::Model()
    : m_VAO(0)
    , m_VBO(0)
    , m_EBO(0)
    , m_Position(0.0f)
    , m_Rotation(0.0f)
    , m_Scale(1.0f)
{
}

Model::~Model() {
    cleanup();
}

bool Model::loadModel(const std::string& filepath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::string directory = filepath.substr(0, filepath.find_last_of('/') + 1);

    // Load the model using tinyobjloader
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str());

    if (!warn.empty()) {
        std::cout << "Warning: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "Error: " << err << std::endl;
        return false;
    }

    if (!ret) {
        std::cerr << "Failed to load model: " << filepath << std::endl;
        return false;
    }

    loadMaterialTextures(materials, directory);

    // Process the loaded data into our mesh format
    processModelData(attrib, shapes, materials);

    // Set up OpenGL buffers
    setupMesh();

    return true;
}

void Model::processModelData(const tinyobj::attrib_t& attrib,
    const std::vector<tinyobj::shape_t>& shapes,
    const std::vector<tinyobj::material_t>& materials) {
    // Clear existing data
    m_Vertices.clear();
    m_Indices.clear();

    // Process all shapes in the model
    for (const auto& shape : shapes) {
        // Process all faces in the shape
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];

            // Process all vertices in the face
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                // Vertex position
                m_Vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                m_Vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                m_Vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

                // Normal
                if (idx.normal_index >= 0) {
                    m_Vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
                    m_Vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
                    m_Vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);
                }
                else {
                    // Default normal if none specified
                    m_Vertices.push_back(0.0f);
                    m_Vertices.push_back(0.0f);
                    m_Vertices.push_back(1.0f);
                }

                // Texture coordinates
                if (idx.texcoord_index >= 0) {
                    m_Vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                    m_Vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                }
                else {
                    // Default texture coordinates if none specified
                    m_Vertices.push_back(0.0f);
                    m_Vertices.push_back(0.0f);
                }

                // Add index
                m_Indices.push_back(m_Indices.size());
            }
            index_offset += fv;
        }
    }
}

void Model::setupMesh() {
    // Create and bind VAO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    // Load vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);

    // Load index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::render() {

    if (!m_Materials.empty()) {
        // Bind the first material's textures
        // In a more advanced implementation, you might want to handle multiple materials
        m_Materials[0]->bind();
    }

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (!m_Materials.empty()) {
        m_Materials[0]->unbind();
    }
}

void Model::cleanup() {
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
}

glm::mat4 Model::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_Position);
    model = glm::rotate(model, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, m_Scale);
    return model;
}

bool Model::loadMaterialTextures(const std::vector<tinyobj::material_t>& materials,
    const std::string& modelPath) {
    for (const auto& material : materials) {
        auto mat = std::make_shared<Material>(material.name);

        // If there's a diffuse texture, load it
        if (!material.diffuse_texname.empty()) {
            std::string texturePath = "gamedata/textures/" + material.diffuse_texname;
            if (!mat->loadDiffuseTexture(texturePath)) {
                std::cerr << "Failed to load texture: " << texturePath << std::endl;
                return false;
            }
        }

        m_Materials.push_back(mat);
    }

    // If no materials were loaded, create a default material
    if (m_Materials.empty()) {
        m_Materials.push_back(std::make_shared<Material>("default"));
    }

    return true;
}