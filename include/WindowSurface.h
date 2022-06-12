#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

class WindowSurface
{
public:
	WindowSurface(const vk::Instance& vulkanInstance, GLFWwindow* glfwWindow);
	~WindowSurface();

	vk::SurfaceKHR getVulkanWindowSurface() const noexcept;

private:
	const vk::Instance vulkanInstance;
	vk::SurfaceKHR vulkanWindowSurface;
};