// camera.cpp
#include "camera.h"

// Constructor sets up default camera values
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
{
    // Calculate initial camera vectors
    updateCameraVectors();

    // Set up mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Get initial window size for mouse position initialization
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
            // Switching to GUI mode
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            // Switching to camera mode
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            // Get the current mouse position and store it
            double xpos, ypos;
            glfwGetCursorPos(m_Window, &xpos, &ypos);
            m_LastX = xpos;
            m_LastY = ypos;
            m_FirstMouse = true;  // This will ensure smooth transition
        }
    }
    wasPressed = isPressed;
}

void Camera::update(float deltaTime) {
    processCursorToggle();    
    processKeyboard(deltaTime);
    processMouseMovement();

    if (!m_CursorEnabled) {
        processKeyboard(deltaTime);
        processMouseMovement();
    }
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

    // Calculate mouse movement since last frame
    float xoffset = xpos - m_LastX;
    float yoffset = m_LastY - ypos; // Reversed since y-coordinates range bottom-to-top

    m_LastX = xpos;
    m_LastY = ypos;

    // Apply sensitivity
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    // Update Euler angles
    m_Yaw += xoffset;
    m_Pitch += yoffset;

    // Constrain pitch
    if (m_Pitch > 89.0f)
        m_Pitch = 89.0f;
    if (m_Pitch < -89.0f)
        m_Pitch = -89.0f;

    // Update camera vectors based on new angles
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // Calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);

    // Recalculate right and up vectors
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}