#include "Framebuffer.h"

Framebuffer::Framebuffer(const vk::Device& vulkanLogicalDevice, const vk::RenderPass& vulkanRenderPass, const vk::ImageView& vulkanSwapChainImageView, const vk::Extent2D& swapChainExtent) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::FramebufferCreateInfo framebufferCreateInfo{ buildFramebufferCreateInfo(vulkanRenderPass, vulkanSwapChainImageView, swapChainExtent) };
	vulkanFramebuffer = vulkanLogicalDevice.createFramebuffer(framebufferCreateInfo);
}

Framebuffer::~Framebuffer()
{
	vulkanLogicalDevice.destroyFramebuffer(vulkanFramebuffer);
}

const vk::FramebufferCreateInfo Framebuffer::buildFramebufferCreateInfo(const vk::RenderPass& vulkanRenderPass, const vk::ImageView& vulkanSwapChainImageView, const vk::Extent2D& swapChainExtent) const
{
	return vk::FramebufferCreateInfo{
		.renderPass = vulkanRenderPass,
		.attachmentCount = 1,
		.pAttachments = &vulkanSwapChainImageView,
		.width = swapChainExtent.width,
		.height = swapChainExtent.height,
		.layers = 1
	};
}

const vk::Framebuffer Framebuffer::getVulkanFramebuffer() const
{
	return vulkanFramebuffer;
}