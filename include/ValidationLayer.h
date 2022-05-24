#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <stdexcept>

class ValidationLayer
{

public:
	ValidationLayer();
	uint32_t getEnabledLayerCount() const;
	char* const* getEnabledLayerNames() const;

private:
	bool isAvailable() const;

	const std::vector<const char*> layerNames = { "VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
	const bool isEnabled{ false };
#else
	const bool isEnabled{ true };
#endif

};