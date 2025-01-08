// camera.h
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "player.h"

class Camera {
private:
    // Camera positioning
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;

    // Euler angles
    float m_Yaw;
    float m_Pitch;

    GLFWwindow* m_Window;

    bool m_FirstMouse;
    float m_LastX;
    float m_LastY;

    void processKeyboard(float deltaTime);
    void processMouseMovement();
    void updateCameraVectors();
    bool m_CursorEnabled;
    void processCursorToggle();

    bool m_AttachedToPlayer;
    Player* m_AttachedPlayer;

public:
    Camera(GLFWwindow* window);
    ~Camera() = default;

    void update(float deltaTime);

    glm::mat4 getViewMatrix() const;

    glm::vec3 getPosition() const { return m_Position; }

    float m_MovementSpeed;
    float m_MouseSensitivity;

    void attachToPlayer(Player* player);
    void detachFromPlayer();
    bool isAttachedToPlayer() const { return m_AttachedToPlayer; }
};
