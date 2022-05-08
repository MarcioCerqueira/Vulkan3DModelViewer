#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

class GraphicInstance
{
public:
	explicit GraphicInstance(const std::string& applicationName);
	~GraphicInstance();

private:
	vk::Instance vulkanInstance;
};