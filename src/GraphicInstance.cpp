#include "GraphicInstance.h"

GraphicInstance::GraphicInstance(const std::string& applicationName)
{
	if (enableValidationLayers && !checkValidationLayerSupport())
	{
		throw std::runtime_error("Validation layers requested, but not available!");
	}
	createVulkanInstance(applicationName);
	physicalDevice.pick(vulkanInstance.enumeratePhysicalDevices());
}

GraphicInstance::~GraphicInstance()
{
	vulkanInstance.destroy();
}

bool GraphicInstance::checkValidationLayerSupport() const
{
	const std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();
	return checkValidationLayerAvailability(validationLayers, availableLayers);
}

bool GraphicInstance::checkValidationLayerAvailability(const std::vector<const char*>& validationLayers, const std::vector<vk::LayerProperties>& availableLayers) const noexcept
{
	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}
		if (!layerFound)
		{
			return false;
		}
	}
	return true;
}

void GraphicInstance::createVulkanInstance(const std::string& applicationName)
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	
	uint32_t enabledLayerCount = (enableValidationLayers) ? static_cast<uint32_t>(validationLayers.size()) : 0;
	const char* const* enabledLayerNames = (enableValidationLayers) ? validationLayers.data() : nullptr;
	
	const vk::ApplicationInfo applicationInfo{ applicationName.c_str(), 1, "No Engine", 1, VK_API_VERSION_1_0 };
	const vk::InstanceCreateInfo createInfo{ {}, &applicationInfo, enabledLayerCount, enabledLayerNames, glfwExtensionCount, glfwExtensions };

	vulkanInstance = vk::createInstance(createInfo);
}


