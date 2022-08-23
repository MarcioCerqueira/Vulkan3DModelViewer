#include "Camera.h"

Camera::Camera()
{
    updateCameraVectors();
}

Camera::Camera(const CameraCreateInfo& cameraCreateInfo) : position(cameraCreateInfo.position), worldUp(cameraCreateInfo.up), yaw(cameraCreateInfo.yaw), pitch(cameraCreateInfo.pitch)
{
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

const float Camera::getZoom() const
{
    return zoom;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    const float velocity{ movementSpeed * deltaTime };
    switch (direction)
    {
    case CameraMovement::FORWARD:
        position += front * velocity;
        break;
    case CameraMovement::BACKWARD:
        position -= front * velocity;
        break;
    case CameraMovement::LEFT:
        position -= right * velocity;
        break;
    case CameraMovement::RIGHT:
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

