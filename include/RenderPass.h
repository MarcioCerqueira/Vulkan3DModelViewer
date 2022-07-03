#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class RenderPass
{
public:
	RenderPass(const vk::Device& vulkanLogicalDevice, const vk::SurfaceFormatKHR& swapChainSurfaceFormat);
	~RenderPass();

	vk::RenderPassBeginInfo createRenderPassBeginInfo(const vk::Framebuffer& vulkanFramebuffer, const vk::Extent2D& swapChainExtent) const;
	vk::RenderPass getVulkanRenderPass() const noexcept;

private:
	vk::RenderPassCreateInfo createRenderPassCreateInfo(const vk::SurfaceFormatKHR& swapChainSurfaceFormat) const;
	vk::AttachmentDescription createAttachmentDescription(const vk::SurfaceFormatKHR& swapChainSurfaceFormat) const;
	vk::SubpassDescription createSubpassDescription() const;
	vk::AttachmentReference createAttachmentReference() const;

	const vk::Device vulkanLogicalDevice;
	vk::RenderPass vulkanRenderPass;
};