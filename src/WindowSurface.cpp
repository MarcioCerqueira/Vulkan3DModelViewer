#include "WindowSurface.h"

WindowSurface::WindowSurface(const vk::Instance& vulkanInstance, GLFWwindow* glfwWindow)
{
	VkSurfaceKHR tempWindowSurface;
	if (glfwCreateWindowSurface(vulkanInstance, glfwWindow, nullptr, &tempWindowSurface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface!");
	}
	vulkanWindowSurface = vk::SurfaceKHR(tempWindowSurface);
}

vk::SurfaceKHR WindowSurface::getVulkanWindowSurface() const noexcept
{
	return vulkanWindowSurface;
}