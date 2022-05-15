#include "GraphicInstance.h"

GraphicInstance::GraphicInstance(const std::string& applicationName)
{
	checkValidationLayerSupport();
	createVulkanInstance(applicationName);

	const std::vector<vk::PhysicalDevice> vulkanPhysicalDevices = vulkanInstance.enumeratePhysicalDevices();
	physicalDevice.pick(vulkanPhysicalDevices);
	
	uint32_t enabledLayerCount = getEnabledLayerCount();
	const char* const* enabledLayerNames = getEnabledLayerNames();
	physicalDevice.createLogicalDevice(enabledLayerCount, enabledLayerNames);
}

GraphicInstance::~GraphicInstance()
{
	vulkanInstance.destroy();
}

void GraphicInstance::checkValidationLayerSupport() const
{
	const std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();
	if (enableValidationLayers && !checkValidationLayerAvailability(validationLayers, availableLayers))
	{
		throw std::runtime_error("Validation layers requested, but not available!");
	}
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
	uint32_t enabledLayerCount = getEnabledLayerCount();
	const char* const* enabledLayerNames = getEnabledLayerNames();
	
	const vk::ApplicationInfo applicationInfo{ applicationName.c_str(), 1, "No Engine", 1, VK_API_VERSION_1_0 };
	const vk::InstanceCreateInfo createInfo{ {}, &applicationInfo, enabledLayerCount, enabledLayerNames, glfwExtensionCount, glfwExtensions };

	vulkanInstance = vk::createInstance(createInfo);
}

uint32_t GraphicInstance::getEnabledLayerCount() const
{
	return (enableValidationLayers) ? static_cast<uint32_t>(validationLayers.size()) : 0;
}

const char* const* GraphicInstance::getEnabledLayerNames() const
{
	return (enableValidationLayers) ? validationLayers.data() : nullptr;
}
