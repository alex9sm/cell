// camera.h
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    // Window reference for input handling
    GLFWwindow* m_Window;

    // Input tracking
    bool m_FirstMouse;
    float m_LastX;
    float m_LastY;

    // Private methods for internal use
    void processKeyboard(float deltaTime);
    void processMouseMovement();
    void updateCameraVectors();
    bool m_CursorEnabled;
    void processCursorToggle();

public:
    Camera(GLFWwindow* window);
    ~Camera() = default;

    // Main update function that handles all input processing
    void update(float deltaTime);

    // Returns the view matrix calculated using Euler angles
    glm::mat4 getViewMatrix() const;

    // Getter for camera position (might be useful for other systems)
    glm::vec3 getPosition() const { return m_Position; }

    // Camera options
    float m_MovementSpeed;
    float m_MouseSensitivity;
};
