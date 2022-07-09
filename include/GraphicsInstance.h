#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <stdexcept>

#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "ValidationLayer.h"
#include "WindowSurface.h"

class GraphicsInstance
{
public:
	explicit GraphicsInstance(const std::string& applicationName);
	~GraphicsInstance();

	void createWindowSurface(GLFWwindow* glfwWindow);
	void selectPhysicalDevice();
	void createLogicalDevice(const WindowSize& framebufferSize);
	void createGraphicsPipeline(const std::vector<std::shared_ptr<Shader>>& shaders);

	const vk::Device getVulkanLogicalDevice() const;

private:
	void createVulkanInstance(const std::string& applicationName);
	const vk::ApplicationInfo createApplicationInfo(const std::string& applicationName) const;
	const vk::InstanceCreateInfo createVulkanInstanceInfo(const vk::ApplicationInfo& applicationInfo) const;

	vk::Instance vulkanInstance;
	PhysicalDevice physicalDevice;
	std::unique_ptr<LogicalDevice> logicalDevice;
	std::unique_ptr<WindowSurface> windowSurface;
	const ValidationLayer validationLayer;
};