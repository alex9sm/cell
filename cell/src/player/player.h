#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

class Player {
private:
    GLFWwindow* m_Window;

    // Position and movement
    glm::vec3 m_Position;
    glm::vec3 m_Velocity;

    // Rotation (in Euler angles)
    float m_Yaw;
    float m_Pitch;

    // Vectors
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;

    // Player properties
    float m_MovementSpeed;
    float m_MouseSensitivity;
    float m_EyeHeight;

    float m_GravityAcceleration;
    float m_TerminalVelocity;

    // AABB properties
    glm::vec3 m_AABBHalfExtents;  // Half-width, half-height, half-depth
    GLuint m_AABBVertexBuffer;
    GLuint m_AABBVertexArray;
    GLuint m_AABBIndexBuffer;

    // Internal methods
    void updateVectors();
    void applyGravity(float deltaTime);
    void setupAABBMesh();

public:
    Player(GLFWwindow* window);
    ~Player();

    void update(float deltaTime);
    void processKeyboard(float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void renderAABB(const Shader& shader) const;

    // Getters for camera attachment
    glm::vec3 getPosition() const { return m_Position + glm::vec3(0.0f, m_EyeHeight, 0.0f); }
    glm::vec3 getFront() const { return m_Front; }
    glm::vec3 getUp() const { return m_Up; }
    float getYaw() const { return m_Yaw; }
    float getPitch() const { return m_Pitch; }

    // AABB collision methods
    glm::vec3 getAABBMin() const { return m_Position - m_AABBHalfExtents; }
    glm::vec3 getAABBMax() const { return m_Position + m_AABBHalfExtents; }
    glm::vec3 getAABBCenter() const { return m_Position; }
    glm::vec3 getAABBHalfExtents() const { return m_AABBHalfExtents; }

    void adjustPosition(const glm::vec3& adjustment) {
        m_Position += adjustment;
    }
};