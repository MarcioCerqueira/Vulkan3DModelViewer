#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vector>

class Framebuffer
{
public:
	Framebuffer(const vk::Device& vulkanLogicalDevice, const vk::RenderPass& vulkanRenderPass, const std::vector<vk::ImageView>& attachments, const vk::Extent2D& swapChainExtent);
	~Framebuffer();

	const vk::Framebuffer getVulkanFramebuffer() const;

private:
	const vk::FramebufferCreateInfo buildFramebufferCreateInfo(const vk::RenderPass& vulkanRenderPass, const std::vector<vk::ImageView>& attachments, const vk::Extent2D& swapChainExtent) const;

	const vk::Device vulkanLogicalDevice;
	vk::Framebuffer vulkanFramebuffer;
};