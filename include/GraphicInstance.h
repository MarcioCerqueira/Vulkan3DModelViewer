#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <stdexcept>

#include "PhysicalDevice.h"
#include "LogicalDevice.h"

class GraphicInstance
{
public:
	explicit GraphicInstance(const std::string& applicationName);
	~GraphicInstance();

	vk::Instance getVulkanInstance() const noexcept;
	void selectPhysicalDevice(const vk::SurfaceKHR& vulkanWindowSurface);
	void createLogicalDevice(const vk::SurfaceKHR& vulkanWindowSurface);

private:
	void checkValidationLayerSupport() const;
	bool checkValidationLayerAvailability(const std::vector<const char*>& validationLayers, const std::vector<vk::LayerProperties>& availableLayers) const noexcept;
	void createVulkanInstance(const std::string& applicationName);
	const uint32_t getEnabledLayerCount() const;
	const char* const* getEnabledLayerNames() const;

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
	const bool enableValidationLayers{ false };
#else
	const bool enableValidationLayers{ true };
#endif
	vk::Instance vulkanInstance;
	PhysicalDevice physicalDevice;
	std::unique_ptr<LogicalDevice> logicalDevice;
};