#pragma once

/*
* Inspired by the implementation available here: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h
*/
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "CameraHandler.h"
#include "structs/CameraCreateInfo.h"

class Camera : public CameraHandler
{
public:
    Camera();
    Camera(const CameraCreateInfo& cameraCreateInfo);
    const glm::mat4 getViewMatrix() const override;
    const float getZoom() const override;
    void processKeyboard(CameraMovement direction, float deltaTime) override;
    void processMouseMovement(float xoffset, float yoffset, bool shouldConstrainPitch) override;
    void processMouseScroll(float yoffset) override;

private:

    void updateCameraVectors();
    void constrainPitch();
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = -89.0f;
    float pitch = 89.0f;
    float movementSpeed = 2.5f;
    float mouseSensitivity = 0.1f;
    float zoom = 45.0f;
};
