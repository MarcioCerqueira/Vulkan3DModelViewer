#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>

#include "SwapChainInfo.h"

class SwapChain
{
public:
	explicit SwapChain(const SwapChainInfo& swapChainInfo);
	bool isValid() const noexcept;

private:
	void checkSwapChainValidity(const std::vector<vk::SurfaceFormatKHR>& availableFormats, const std::vector<vk::PresentModeKHR>& availablePresentModes);
	void chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	void chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
	void chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const WindowSize& framebufferSize);
	void setImageCount(const vk::SurfaceCapabilitiesKHR& capabilities);
	void createVulkanSwapChainInfo(const SwapChainInfo& swapChainInfo, const vk::SurfaceCapabilitiesKHR& capabilities);

	vk::SwapchainKHR vulkanSwapChain; 
	std::vector<vk::Image> swapChainImages;
	vk::SurfaceFormatKHR surfaceFormat;
	vk::PresentModeKHR presentMode;
	vk::Extent2D extent;
	uint32_t imageCount;
	bool valid;
};