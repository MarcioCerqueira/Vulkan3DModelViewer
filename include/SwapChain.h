#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>

#include "structs/SwapChainCreateInfo.h"
#include "structs/SwapChainRecreateInfo.h"
#include "ExceptionChecker.h"
#include "Framebuffer.h"
#include "ImageView.h"

class SwapChain
{
public:
	explicit SwapChain(const SwapChainCreateInfo& swapChainCreateInfo);
	~SwapChain();

	static const bool isValid(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::SurfaceKHR& vulkanWindowSurface);
	vk::Result acquireNextImage(vk::Semaphore& imageAvailable, const vk::RenderPass& vulkanRenderPass, uint32_t& imageIndex);
	void buildFramebuffers(const vk::Device& vulkanLogicalDevice, const vk::RenderPass& vulkanRenderPass);
	void recreateIfResultIsOutOfDateOrSuboptimalKHR(vk::Result& result, const SwapChainRecreateInfo& swapChainRecreateInfo);

	const vk::Extent2D getExtent() const;
	const vk::Framebuffer getVulkanFramebuffer(const int framebufferIndex) const;
	const vk::SurfaceFormatKHR getSurfaceFormat() const;
	const vk::SwapchainKHR getVulkanSwapChain() const;

private:
	void chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	void chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
	void chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const WindowSize& framebufferSize);
	const uint32_t estimateImageCount(const vk::SurfaceCapabilitiesKHR& capabilities);
	void buildVulkanSwapChain(const SwapChainCreateInfo& swapChainCreateInfo, const vk::SurfaceCapabilitiesKHR& capabilities, const uint32_t imageCount);
	void buildSwapChainImageViews(const SwapChainCreateInfo& swapChainCreateInfo); 
	void waitValidFramebufferSize(std::function<WindowSize()> getFramebufferSize, std::function<void()> waitEvents);
	void cleanup();

	const SwapChainCreateInfo swapChainCreateInfo;
	vk::SwapchainKHR vulkanSwapChain; 
	std::vector<vk::Image> images; 
	std::vector<std::unique_ptr<ImageView>> imageViews;
	std::vector<std::unique_ptr<Framebuffer>> framebuffers;
	vk::SurfaceFormatKHR surfaceFormat;
	vk::PresentModeKHR presentMode;
	vk::Extent2D extent;
};