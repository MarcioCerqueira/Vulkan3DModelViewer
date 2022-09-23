#pragma once

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>
#include "structs/ModelViewProjectionTransformation.h"

enum class CameraMovement 
{
    Forward,
    Backward,
    Left,
    Right
};

enum class MouseButton
{
    None,
    Left,
    Middle,
    Right
};

enum class Action
{
    None,
    Orbit,
    Dolly,
    Pan,
    LookAround
};

enum class MouseModifierFlagBits : uint32_t
{
    Shift = 1,
    Ctrl = 2,
    Alt = 4
};

using MouseModifierFlags = vk::Flags<MouseModifierFlagBits>;

class CameraHandler
{
public:
	virtual ModelViewProjectionTransformation getMVPTransformation() const = 0;
    virtual void processKeyboard(CameraMovement direction, float deltaTime) = 0;
    virtual Action processMouseMovement(const glm::ivec2& position, MouseButton mouseButton, MouseModifierFlags& modifiers) = 0;
    virtual void setMousePosition(const glm::ivec2& position) = 0;
    virtual void processMouseScroll(int value) = 0;
    virtual void setWindowSize(const glm::ivec2& size) = 0;
    virtual void printCameraData() const = 0;
};