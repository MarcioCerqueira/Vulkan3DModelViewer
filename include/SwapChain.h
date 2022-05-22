#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>

#include "WindowSize.h"

class SwapChain
{
public:
	SwapChain(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface, const WindowSize& framebufferSize);
	bool isValid() const noexcept;

private:
	void checkSwapChainValidity(const std::vector<vk::SurfaceFormatKHR>& availableFormats, const std::vector<vk::PresentModeKHR>& availablePresentModes);
	void chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	void chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
	void chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const WindowSize& framebufferSize);

	vk::SurfaceFormatKHR format;
	vk::PresentModeKHR presentMode;
	vk::Extent2D extent;
	bool valid;
};