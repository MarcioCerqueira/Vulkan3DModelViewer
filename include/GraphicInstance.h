#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <stdexcept>

class GraphicInstance
{
public:
	explicit GraphicInstance(const std::string& applicationName);
	~GraphicInstance();

private:
	bool checkValidationLayerSupport();
	bool checkValidationLayerAvailability(const std::vector<const char*>& validationLayers, const std::vector<vk::LayerProperties>& availableLayers);
	void createVulkanInstance(const std::string& applicationName);

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
	vk::Instance vulkanInstance;

};