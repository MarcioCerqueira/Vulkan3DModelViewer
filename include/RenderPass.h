#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class RenderPass
{
public:
	RenderPass(const vk::Device& vulkanLogicalDevice, const vk::SurfaceFormatKHR& swapChainSurfaceFormat);

private:
	vk::AttachmentDescription createAttachmentDescription(const vk::SurfaceFormatKHR& swapChainSurfaceFormat) const;
	vk::AttachmentReference createAttachmentReference() const;
	vk::SubpassDescription createSubpassDescription(const vk::AttachmentReference& attachmentReference) const;
	vk::RenderPassCreateInfo createRenderPassCreateInfo(const vk::AttachmentDescription& attachmentDescription, const vk::SubpassDescription& subpassDescription);

	vk::RenderPass vulkanRenderPass;
};