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
	vk::Device getVulkanLogicalDevice() const;
	vk::Extent2D getSwapChainExtent() const;
	vk::SurfaceFormatKHR getSwapChainSurfaceFormat() const;

	void selectPhysicalDevice(const vk::SurfaceKHR& vulkanWindowSurface);
	void createLogicalDevice(const vk::SurfaceKHR& vulkanWindowSurface, const WindowSize& framebufferSize);

private:
	void createVulkanInstance(const std::string& applicationName);
	vk::ApplicationInfo createApplicationInfo(const std::string& applicationName) const;
	vk::InstanceCreateInfo createVulkanInstanceInfo(const vk::ApplicationInfo& applicationInfo) const;

	vk::Instance vulkanInstance;
	PhysicalDevice physicalDevice;
	std::unique_ptr<LogicalDevice> logicalDevice;
	const ValidationLayer validationLayer;
};