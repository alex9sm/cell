#pragma once
#include "texture.h"
#include <memory>
#include <string>
#include <glm/glm.hpp>

class Material {
private:
    std::shared_ptr<Texture> m_DiffuseTexture;
    std::string m_Name;

    glm::vec3 m_Ambient;
    glm::vec3 m_Diffuse;
    float m_Shininess;

public:

    Material(const std::string& name = "default")
        : m_Name(name)
        , m_DiffuseTexture(nullptr)
        , m_Ambient(0.1f)
        , m_Diffuse(0.8f)
        , m_Shininess(32.0f)
    {}

    ~Material() = default;

    Material(const Material&) = delete;
    Material& operator=(const Material&) = delete;

    bool loadDiffuseTexture(const std::string& path);
    void bind() const;
    void unbind() const;

    const std::string& getName() const { return m_Name; }
    std::shared_ptr<Texture> getDiffuseTexture() const { return m_DiffuseTexture; }

    void setAmbient(const glm::vec3& ambient) { m_Ambient = ambient; }
    void setDiffuse(const glm::vec3& diffuse) { m_Diffuse = diffuse; }
    void setShininess(float shininess) { m_Shininess = shininess; }
};