#pragma once
#include "texture.h"
#include <memory>
#include <string>

class Material {
private:
    std::shared_ptr<Texture> m_DiffuseTexture;
    std::string m_Name;

public:
    Material(const std::string& name = "default");
    ~Material() = default;

    // Prevent copying of material objects
    Material(const Material&) = delete;
    Material& operator=(const Material&) = delete;

    // Core functionality
    bool loadDiffuseTexture(const std::string& path);
    void bind() const;
    void unbind() const;

    // Getters
    const std::string& getName() const { return m_Name; }
    std::shared_ptr<Texture> getDiffuseTexture() const { return m_DiffuseTexture; }
};