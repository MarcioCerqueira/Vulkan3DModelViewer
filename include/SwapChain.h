#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>

#include "SwapChainCreateInfo.h"

class SwapChain
{
public:
	explicit SwapChain(const SwapChainCreateInfo& swapChainCreateInfo);
	static bool isValid(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface);

private:
	void chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	void chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
	void chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const WindowSize& framebufferSize);
	void setImageCount(const vk::SurfaceCapabilitiesKHR& capabilities);
	void buildVulkanSwapChainCreateInfo(const SwapChainCreateInfo& swapChainCreateInfo, const vk::SurfaceCapabilitiesKHR& capabilities);

	vk::SwapchainKHR vulkanSwapChain; 
	std::vector<vk::Image> swapChainImages;
	vk::SurfaceFormatKHR surfaceFormat;
	vk::PresentModeKHR presentMode;
	vk::Extent2D extent;
	uint32_t imageCount;
};