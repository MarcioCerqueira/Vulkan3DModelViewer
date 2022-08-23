#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front(glm::vec3(-2.0f, -2.0f, -2.0f)), movementSpeed(2.5f), mouseSensitivity(0.1f), zoom(45.0f)
{
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(-2.0f, -2.0f, -2.0f)), movementSpeed(2.5f), mouseSensitivity(0.1f), zoom(45.0f)
{
    this->position = glm::vec3(posX, posY, posZ);
    this->worldUp = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    glm::vec3 unnormalizedFront;
    unnormalizedFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    unnormalizedFront.y = sin(glm::radians(pitch));
    unnormalizedFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(unnormalizedFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

const glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    const float velocity{ movementSpeed * deltaTime };
    switch (direction)
    {
    case FORWARD:
        position += front * velocity;
        break;
    case BACKWARD:
        position -= front * velocity;
        break;
    case LEFT:
        position -= right * velocity;
        break;
    case RIGHT:
        position += right * velocity;
        break;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool shouldConstrainPitch)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (shouldConstrainPitch)
    {
        constrainPitch();
    }
    updateCameraVectors();
}

void Camera::constrainPitch()
{
    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }
}

void Camera::processMouseScroll(float yoffset)
{
    zoom -= yoffset;
    if (zoom < 1.0f)
    {
        zoom = 1.0f;
    }
    if (zoom > 45.0f)
    {
        zoom = 45.0f;
    }
}

