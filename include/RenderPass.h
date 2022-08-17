#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class RenderPass
{
public:
	RenderPass(const vk::Device& vulkanLogicalDevice, const vk::Format& colorImageFormat, const vk::Format& depthImageFormat);
	~RenderPass();

	const vk::RenderPassBeginInfo createRenderPassBeginInfo(const vk::Framebuffer& vulkanFramebuffer, const vk::Extent2D& swapChainExtent) const;
	const vk::RenderPass getVulkanRenderPass() const;

private:
	const vk::AttachmentDescription createColorAttachmentDescription(const vk::Format& colorImageFormat) const;
	const vk::AttachmentDescription createDepthAttachmentDescription(const vk::Format& depthImageFormat) const;
	const vk::SubpassDescription createSubpassDescription(const vk::AttachmentReference& colorAttachmentReference, const vk::AttachmentReference& depthAttachmentReference) const;
	const vk::AttachmentReference createColorAttachmentReference() const;
	const vk::AttachmentReference createDepthAttachmentReference() const;
	const vk::SubpassDependency createSubpassDependency() const;
	const vk::RenderPassCreateInfo createRenderPassCreateInfo(const std::vector<vk::AttachmentDescription>& attachmentDescriptions, const vk::SubpassDescription& subpassDescription, const vk::SubpassDependency& subpassDependency) const;

	const vk::Device vulkanLogicalDevice;
	vk::RenderPass vulkanRenderPass;
};