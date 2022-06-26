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
	~SwapChain();

	static bool isValid(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface);
	vk::Extent2D getExtent() const noexcept;
	vk::SurfaceFormatKHR getSurfaceFormat() const noexcept;
	int getNumberOfImageViews() const noexcept;
	vk::ImageView getImageView(int index) const;

private:
	void chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	void chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
	void chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const WindowSize& framebufferSize);
	uint32_t estimateImageCount(const vk::SurfaceCapabilitiesKHR& capabilities);
	void buildVulkanSwapChain(const SwapChainCreateInfo& swapChainCreateInfo, const vk::SurfaceCapabilitiesKHR& capabilities, const uint32_t imageCount);
	void buildSwapChainImageViews(const SwapChainCreateInfo& swapChainCreateInfo);
	vk::ImageViewCreateInfo buildImageViewCreateInfo(const int imageIndex) const;
	vk::ImageSubresourceRange createImageSubresourceRange() const;

	const SwapChainCreateInfo swapChainCreateInfo;
	vk::SwapchainKHR vulkanSwapChain; 
	std::vector<vk::Image> images; 
	std::vector<vk::ImageView> imageViews;
	vk::SurfaceFormatKHR surfaceFormat;
	vk::PresentModeKHR presentMode;
	vk::Extent2D extent;
};