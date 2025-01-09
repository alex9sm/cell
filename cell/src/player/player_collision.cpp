#include "player_collision.h"
#include <glm/gtc/matrix_transform.hpp>

PlayerCollision::PlayerCollision(ModelManager& modelManager, Player& player)
    : m_ModelManager(modelManager)
    , m_Player(player)
{
}

void PlayerCollision::update() {
    // Update collision geometry from models
    updateWorldSpaceTriangles();

    // Test for collisions and resolve them
    glm::vec3 penetrationVector(0.0f);
    bool hasCollision = false;

    // Test against each triangle
    for (const auto& triangle : m_WorldSpaceTriangles) {
        glm::vec3 trianglePenetration(0.0f);
        if (testAABBTriangleCollision(triangle, trianglePenetration)) {
            hasCollision = true;
            // Accumulate penetration vectors
            penetrationVector += trianglePenetration;
        }
    }

    if (hasCollision) {
        resolveCollision(penetrationVector);
    }
}

void PlayerCollision::updateWorldSpaceTriangles() {
    m_WorldSpaceTriangles.clear();

    // Get loaded models from model manager
    const auto& models = m_ModelManager.getLoadedModels();

    // Process each model
    for (const auto& model : models) {
        // Get model's transformation matrix
        glm::mat4 modelMatrix = model->getModelMatrix();
        extractTrianglesFromModel(*model, modelMatrix);
    }
}

void PlayerCollision::extractTrianglesFromModel(const Model& model, const glm::mat4& modelMatrix) {
    const std::vector<float>& vertices = model.getVertices();
    const std::vector<unsigned int>& indices = model.getIndices();

    // Process each triangle (every 3 indices)
    for (size_t i = 0; i < indices.size(); i += 3) {
        CollisionTriangle triangle;

        // Get triangle vertices from interleaved vertex data (stride of 8: 3 pos, 3 normal, 2 tex)
        size_t idx0 = indices[i] * 8;
        size_t idx1 = indices[i + 1] * 8;
        size_t idx2 = indices[i + 2] * 8;

        // Transform vertices to world space
        glm::vec4 v0(vertices[idx0], vertices[idx0 + 1], vertices[idx0 + 2], 1.0f);
        glm::vec4 v1(vertices[idx1], vertices[idx1 + 1], vertices[idx1 + 2], 1.0f);
        glm::vec4 v2(vertices[idx2], vertices[idx2 + 1], vertices[idx2 + 2], 1.0f);

        triangle.v0 = glm::vec3(modelMatrix * v0);
        triangle.v1 = glm::vec3(modelMatrix * v1);
        triangle.v2 = glm::vec3(modelMatrix * v2);

        // Calculate triangle normal
        glm::vec3 edge1 = triangle.v1 - triangle.v0;
        glm::vec3 edge2 = triangle.v2 - triangle.v0;
        triangle.normal = glm::normalize(glm::cross(edge1, edge2));

        m_WorldSpaceTriangles.push_back(triangle);
    }
}

bool PlayerCollision::testAABBTriangleCollision(const CollisionTriangle& triangle, glm::vec3& penetrationVector) const {
    // Get AABB data
    glm::vec3 aabbCenter = m_Player.getAABBCenter();
    glm::vec3 aabbHalfExtents = m_Player.getAABBHalfExtents();

    // Test AABB against triangle using Separating Axis Theorem (SAT)

    // 1. Test triangle normal axis
    float radius = glm::dot(aabbHalfExtents, glm::abs(triangle.normal));
    float distance = glm::dot(triangle.normal, aabbCenter - triangle.v0);

    if (std::abs(distance) > radius) {
        return false;
    }

    // 2. Test AABB face normals
    const glm::vec3 axes[3] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    };

    for (const auto& axis : axes) {
        float p0 = glm::dot(triangle.v0, axis);
        float p1 = glm::dot(triangle.v1, axis);
        float p2 = glm::dot(triangle.v2, axis);

        float min = std::min({ p0, p1, p2 });
        float max = std::max({ p0, p1, p2 });

        float boxMin = glm::dot(aabbCenter, axis) - aabbHalfExtents[0];
        float boxMax = glm::dot(aabbCenter, axis) + aabbHalfExtents[0];

        if (boxMin > max || boxMax < min) {
            return false;
        }
    }

    // 3. Test edge cross products
    glm::vec3 edges[3] = {
        triangle.v1 - triangle.v0,
        triangle.v2 - triangle.v1,
        triangle.v0 - triangle.v2
    };

    for (const auto& edge : edges) {
        for (const auto& axis : axes) {
            glm::vec3 crossAxis = glm::cross(edge, axis);
            if (glm::length(crossAxis) > 0.0001f) { // Avoid near-zero axes
                crossAxis = glm::normalize(crossAxis);

                // Project triangle vertices
                float p0 = glm::dot(triangle.v0, crossAxis);
                float p1 = glm::dot(triangle.v1, crossAxis);
                float p2 = glm::dot(triangle.v2, crossAxis);

                float triMin = std::min({ p0, p1, p2 });
                float triMax = std::max({ p0, p1, p2 });

                // Project AABB
                float boxProjection = glm::dot(aabbHalfExtents, glm::abs(crossAxis));
                float boxCenter = glm::dot(aabbCenter, crossAxis);

                float boxMin = boxCenter - boxProjection;
                float boxMax = boxCenter + boxProjection;

                if (boxMin > triMax || boxMax < triMin) {
                    return false;
                }
            }
        }
    }

    // Calculate penetration vector
    penetrationVector = triangle.normal * (radius - std::abs(distance));
    return true;
}

void PlayerCollision::resolveCollision(const glm::vec3& penetrationVector) {
    // Move player out of collision
    if (glm::length(penetrationVector) > 0.0f) {
        m_Player.adjustPosition(penetrationVector);
    }
}

void PlayerCollision::renderCollisionGeometry(const Shader& shader) const {
    // Debug rendering of collision triangles
    for (const auto& triangle : m_WorldSpaceTriangles) {
        // Set up transformation and color
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f)); // Red for collision geometry

        // Draw triangle wireframe
        glBegin(GL_LINE_LOOP);
        glVertex3fv(&triangle.v0[0]);
        glVertex3fv(&triangle.v1[0]);
        glVertex3fv(&triangle.v2[0]);
        glEnd();
    }
}