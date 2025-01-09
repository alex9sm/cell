#include "player.h"
#include "ui.h"

Player::Player(GLFWwindow* window)
    : m_Window(window)
    , m_Position(glm::vec3(0.0f, 5.0f, 0.0f))
    , m_Velocity(glm::vec3(0.0f))
    , m_Front(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
    , m_Yaw(-90.0f)
    , m_Pitch(0.0f)
    , m_MovementSpeed(2.5f)
    , m_MouseSensitivity(0.1f)
    , m_EyeHeight(1.8f)
    , m_GravityAcceleration(9.81f)
    , m_TerminalVelocity(53.0f)
    , m_AABBHalfExtents(glm::vec3(0.4f, 0.4f, 0.4f)) // Slightly smaller than OBB for better fit
{
    updateVectors();
    setupAABBMesh();
}

Player::~Player() {
    glDeleteBuffers(1, &m_AABBVertexBuffer);
    glDeleteBuffers(1, &m_AABBIndexBuffer);
    glDeleteVertexArrays(1, &m_AABBVertexArray);
}

void Player::update(float deltaTime) {
    processKeyboard(deltaTime);
    applyGravity(deltaTime);

    // Update position based on velocity
    m_Position += m_Velocity * deltaTime;
}

void Player::applyGravity(float deltaTime) {
    m_Velocity.y -= m_GravityAcceleration * deltaTime;

    // Clamp to terminal velocity
    if (m_Velocity.y < -m_TerminalVelocity) {
        m_Velocity.y = -m_TerminalVelocity;
    }
}

void Player::processKeyboard(float deltaTime) {
    float velocity = m_MovementSpeed * deltaTime;

    float yVelocity = m_Velocity.y;

    // Reset velocity for horizontal movement
    m_Velocity = glm::vec3(0.0f);

    // Forward/Backward
    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        m_Velocity += m_Front * velocity;
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        m_Velocity -= m_Front * velocity;

    // Left/Right
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        m_Velocity -= m_Right * velocity;
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        m_Velocity += m_Right * velocity;

    // Remove any y component from horizontal movement
    m_Velocity.y = 0.0f;

    // Normalize horizontal movement if moving diagonally
    if (glm::length(m_Velocity) > 0.0f) {
        m_Velocity = glm::normalize(m_Velocity) * m_MovementSpeed;
    }

    // Restore the y velocity from gravity
    m_Velocity.y = yVelocity;
}

void Player::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    // Constrain the pitch
    if (constrainPitch) {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    updateVectors();
}

void Player::updateVectors() {
    // Calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);

    // Re-calculate the Right and Up vectors
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

void Player::setupAABBMesh() {
    // AABB vertices
    float vertices[] = {
        // Front face
        -0.5f, -1.0f, -0.5f,
         0.5f, -1.0f, -0.5f,
         0.5f,  1.0f, -0.5f,
        -0.5f,  1.0f, -0.5f,
        // Back face
        -0.5f, -1.0f,  0.5f,
         0.5f, -1.0f,  0.5f,
         0.5f,  1.0f,  0.5f,
        -0.5f,  1.0f,  0.5f,
    };

    // Indices for drawing the box as lines
    unsigned int indices[] = {
        // Front face
        0, 1,    1, 2,    2, 3,    3, 0,
        // Back face
        4, 5,    5, 6,    6, 7,    7, 4,
        // Connecting lines
        0, 4,    1, 5,    2, 6,    3, 7
    };

    glGenVertexArrays(1, &m_AABBVertexArray);
    glGenBuffers(1, &m_AABBVertexBuffer);
    glGenBuffers(1, &m_AABBIndexBuffer);

    glBindVertexArray(m_AABBVertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_AABBVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_AABBIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Player::renderAABB(const Shader& shader) const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_Position + glm::vec3(0.0f, m_AABBHalfExtents.y, 0.0f));
    model = glm::scale(model, m_AABBHalfExtents * 2.0f); // Double half-extents to get full size

    shader.setMat4("model", model);
    shader.setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f)); // Green wireframe

    glBindVertexArray(m_AABBVertexArray);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}