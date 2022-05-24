#include "ValidationLayer.h"

ValidationLayer::ValidationLayer()
{
	if (isEnabled && !isAvailable())
	{
		throw std::runtime_error("Validation layer requested, but not available!");
	}
}

bool ValidationLayer::isAvailable() const
{
	const std::vector<vk::LayerProperties> availableLayers{ vk::enumerateInstanceLayerProperties() };
	for (const char* layerName : layerNames)
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

uint32_t ValidationLayer::getEnabledLayerCount() const
{
	return (isEnabled) ? static_cast<uint32_t>(layerNames.size()) : 0;
}

char* const* ValidationLayer::getEnabledLayerNames() const
{
	return (isEnabled) ? const_cast<char* const*>(layerNames.data()) : nullptr;
}