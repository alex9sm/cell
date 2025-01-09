#pragma once
#include "model_manager.h"
#include "player.h"
#include <vector>
#include <glm/glm.hpp>

// Represents a triangle in world space for collision detection
struct CollisionTriangle {
    glm::vec3 v0, v1, v2;  // Vertices in world space
    glm::vec3 normal;      // Triangle normal
};

class PlayerCollision {
private:
    ModelManager& m_ModelManager;
    Player& m_Player;
    std::vector<CollisionTriangle> m_WorldSpaceTriangles;

    // Collision detection helpers
    bool testAABBTriangleCollision(const CollisionTriangle& triangle, glm::vec3& penetrationVector) const;
    void extractTrianglesFromModel(const Model& model, const glm::mat4& modelMatrix);
    void updateWorldSpaceTriangles();

    // Collision response
    void resolveCollision(const glm::vec3& penetrationVector);

public:
    PlayerCollision(ModelManager& modelManager, Player& player);
    ~PlayerCollision() = default;

    // Delete copy constructor and assignment operator
    PlayerCollision(const PlayerCollision&) = delete;
    PlayerCollision& operator=(const PlayerCollision&) = delete;

    // Core functionality
    void update();

    // Debug rendering
    void renderCollisionGeometry(const Shader& shader) const;
};