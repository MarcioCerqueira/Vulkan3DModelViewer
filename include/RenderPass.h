#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class RenderPass
{
public:
	RenderPass(const vk::Device& vulkanLogicalDevice, const vk::SurfaceFormatKHR& swapChainSurfaceFormat);
	~RenderPass();

	const vk::RenderPassBeginInfo createRenderPassBeginInfo(const vk::Framebuffer& vulkanFramebuffer, const vk::Extent2D& swapChainExtent) const;
	const vk::RenderPass getVulkanRenderPass() const;

private:
	const vk::RenderPassCreateInfo createRenderPassCreateInfo(const vk::AttachmentDescription& attachmentDescription, const vk::SubpassDescription& subpassDescription, const vk::SubpassDependency& subpassDependency) const;
	const vk::AttachmentDescription createAttachmentDescription(const vk::SurfaceFormatKHR& swapChainSurfaceFormat) const;
	const vk::SubpassDescription createSubpassDescription(const vk::AttachmentReference& attachmentReference) const;
	const vk::AttachmentReference createAttachmentReference() const;
	const vk::SubpassDependency createSubpassDependency() const;

	const vk::Device vulkanLogicalDevice;
	vk::RenderPass vulkanRenderPass;
};