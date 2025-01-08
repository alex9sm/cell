#include "camera.h"

// default camera values
Camera::Camera(GLFWwindow* window)
    : m_Window(window)
    , m_Position(glm::vec3(0.0f, 0.0f, 3.0f))
    , m_Front(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
    , m_Yaw(-90.0f)
    , m_Pitch(0.0f)
    , m_MovementSpeed(2.5f)
    , m_MouseSensitivity(0.1f)
    , m_FirstMouse(true)
    , m_LastX(0.0f)
    , m_LastY(0.0f)
    , m_CursorEnabled(false)
    , m_AttachedToPlayer(false)
    , m_AttachedPlayer(nullptr)

{
    updateCameraVectors();

    // mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    m_LastX = width / 2.0f;
    m_LastY = height / 2.0f;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Camera::processCursorToggle() {
    static bool wasPressed = false;
    bool isPressed = glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

    if (isPressed && !wasPressed) {
        m_CursorEnabled = !m_CursorEnabled;

        if (m_CursorEnabled) {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            double xpos, ypos;
            glfwGetCursorPos(m_Window, &xpos, &ypos);
            m_LastX = xpos;
            m_LastY = ypos;
            m_FirstMouse = true;  
        }
    }
    wasPressed = isPressed;
}

void Camera::processKeyboard(float deltaTime) {

    if (m_CursorEnabled) {
        return;
    }

    float velocity = m_MovementSpeed * deltaTime;

    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        m_Position += m_Front * velocity;

    // Forward/Backward
    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        m_Position += m_Front * velocity;
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        m_Position -= m_Front * velocity;

    // Left/Right
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        m_Position -= m_Right * velocity;
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        m_Position += m_Right * velocity;

    // Up/Down
    if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_Position += m_WorldUp * velocity;
    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_Position -= m_WorldUp * velocity;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::attachToPlayer(Player* player) {
    m_AttachedPlayer = player;
    m_AttachedToPlayer = true;

    // Sync camera with player position and rotation
    m_Position = m_AttachedPlayer->getPosition();
    m_Front = m_AttachedPlayer->getFront();
    m_Up = m_AttachedPlayer->getUp();
    m_Yaw = m_AttachedPlayer->getYaw();
    m_Pitch = m_AttachedPlayer->getPitch();
}

void Camera::detachFromPlayer() {
    m_AttachedToPlayer = false;
    m_AttachedPlayer = nullptr;
}

// Modify the update function in camera.cpp:
void Camera::update(float deltaTime) {
    processCursorToggle();

    if (m_AttachedToPlayer) {
        // When attached to player, sync position and rotation
        m_Position = m_AttachedPlayer->getPosition();
        m_Front = m_AttachedPlayer->getFront();
        m_Up = m_AttachedPlayer->getUp();
        m_Yaw = m_AttachedPlayer->getYaw();
        m_Pitch = m_AttachedPlayer->getPitch();

        // Pass mouse movement to player
        if (!m_CursorEnabled) {
            processMouseMovement();
        }
    }
    else {
        // Regular free camera movement
        if (!m_CursorEnabled) {
            processKeyboard(deltaTime);
            processMouseMovement();
        }
    }
}

// Modify processMouseMovement in camera.cpp:
void Camera::processMouseMovement() {
    if (m_CursorEnabled) {
        return;
    }

    double xpos, ypos;
    glfwGetCursorPos(m_Window, &xpos, &ypos);

    if (m_FirstMouse) {
        m_LastX = xpos;
        m_LastY = ypos;
        m_FirstMouse = false;
    }

    float xoffset = xpos - m_LastX;
    float yoffset = m_LastY - ypos;

    m_LastX = xpos;
    m_LastY = ypos;

    if (m_AttachedToPlayer) {
        // Pass mouse movement to player
        m_AttachedPlayer->processMouseMovement(xoffset, yoffset);
    }
    else {
        // Regular camera movement
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;

        updateCameraVectors();
    }
}