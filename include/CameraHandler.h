#pragma once

#include <glm/glm.hpp>

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class CameraHandler
{
public:
	virtual const glm::mat4 getViewMatrix() const = 0;
    virtual const float getZoom() const = 0;
    virtual void processKeyboard(CameraMovement direction, float deltaTime) = 0;
	virtual void processMouseMovement(float xoffset, float yoffset, bool shouldConstrainPitch) = 0;
    virtual void processMouseScroll(float yoffset) = 0;
};