#include "Framebuffer.h"

Framebuffer::Framebuffer(const vk::Device& vulkanLogicalDevice, const vk::RenderPass& vulkanRenderPass, const std::vector<vk::ImageView>& attachments, const vk::Extent2D& swapChainExtent) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::FramebufferCreateInfo framebufferCreateInfo{ buildFramebufferCreateInfo(vulkanRenderPass, attachments, swapChainExtent) };
	vulkanFramebuffer = vulkanLogicalDevice.createFramebuffer(framebufferCreateInfo);
}

Framebuffer::~Framebuffer()
{
	vulkanLogicalDevice.destroyFramebuffer(vulkanFramebuffer);
}

const vk::FramebufferCreateInfo Framebuffer::buildFramebufferCreateInfo(const vk::RenderPass& vulkanRenderPass, const std::vector<vk::ImageView>& attachments, const vk::Extent2D& swapChainExtent) const
{
	return vk::FramebufferCreateInfo{
		.renderPass = vulkanRenderPass,
		.attachmentCount = static_cast<uint32_t>(attachments.size()),
		.pAttachments = attachments.data(),
		.width = swapChainExtent.width,
		.height = swapChainExtent.height,
		.layers = 1
	};
}

const vk::Framebuffer Framebuffer::getVulkanFramebuffer() const
{
	return vulkanFramebuffer;
}