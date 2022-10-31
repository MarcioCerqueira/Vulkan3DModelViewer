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
	auto validationLayerIsNotAvailable = std::ranges::find_if(layerNames, [availableLayers](const char* layerName) {
		auto layerFound = std::ranges::find_if(availableLayers, [layerName](const auto& layerProperties) {
			return strcmp(layerName, layerProperties.layerName) == 0;
		});
		return layerFound == availableLayers.end();
	});
	return validationLayerIsNotAvailable == layerNames.end();
}

uint32_t ValidationLayer::getEnabledLayerCount() const
{
	return (isEnabled) ? static_cast<uint32_t>(layerNames.size()) : 0;
}

char* const* ValidationLayer::getEnabledLayerNames() const
{
	return (isEnabled) ? const_cast<char* const*>(layerNames.data()) : nullptr;
}