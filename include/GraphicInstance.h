#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <stdexcept>

#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "ValidationLayer.h"

class GraphicInstance
{
public:
	explicit GraphicInstance(const std::string& applicationName);
	~GraphicInstance();

	vk::Instance getVulkanInstance() const noexcept;
	void selectPhysicalDevice(const vk::SurfaceKHR& vulkanWindowSurface, const WindowSize& framebufferSize);
	void createLogicalDevice(const vk::SurfaceKHR& vulkanWindowSurface);

private:
	void createVulkanInstance(const std::string& applicationName);
	
	vk::Instance vulkanInstance;
	PhysicalDevice physicalDevice;
	std::unique_ptr<LogicalDevice> logicalDevice;
	const ValidationLayer validationLayer;
};