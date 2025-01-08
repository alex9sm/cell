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
    float m_EyeHeight;  // Offset for camera position

    float m_GravityAcceleration;  // Acceleration due to gravity (m/s^2)
    float m_TerminalVelocity;     // Maximum falling speed (m/s)

    // Internal methods
    void updateVectors();
    void applyGravity(float deltaTime);

public:
    Player(GLFWwindow* window);
    ~Player() = default;

    // Main update function
    void update(float deltaTime);

    // Input processing
    void processKeyboard(float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // Getters for camera attachment
    glm::vec3 getPosition() const { return m_Position + glm::vec3(0.0f, m_EyeHeight, 0.0f); }
    glm::vec3 getFront() const { return m_Front; }
    glm::vec3 getUp() const { return m_Up; }
    float getYaw() const { return m_Yaw; }
    float getPitch() const { return m_Pitch; }
};
