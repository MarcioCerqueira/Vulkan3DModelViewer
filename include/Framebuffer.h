#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vector>

class Framebuffer
{
public:
	Framebuffer(const vk::Device& vulkanLogicalDevice, const vk::RenderPass& vulkanRenderPass, const vk::ImageView& vulkanSwapChainImageView, const vk::Extent2D& swapChainExtent);
	~Framebuffer();

	vk::Framebuffer getVulkanFramebuffer() const noexcept;

private:
	vk::FramebufferCreateInfo buildFramebufferCreateInfo(const vk::RenderPass& vulkanRenderPass, const vk::ImageView& vulkanSwapChainImageView, const vk::Extent2D& swapChainExtent) const;

	const vk::Device vulkanLogicalDevice;
	vk::Framebuffer vulkanFramebuffer;
};