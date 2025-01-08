#include "material.h"

bool Material::loadDiffuseTexture(const std::string& path) {
    m_DiffuseTexture = std::make_shared<Texture>();
    return m_DiffuseTexture->loadTexture(path);
}

void Material::bind() const {
    if (m_DiffuseTexture) {
        m_DiffuseTexture->bind(0); // unit 0 for diffuse
    }
}

void Material::unbind() const {
    if (m_DiffuseTexture) {
        m_DiffuseTexture->unbind();
    }
}