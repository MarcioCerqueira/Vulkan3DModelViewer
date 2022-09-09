#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "structs/RenderPassInfo.h"

class RenderPass
{
public:
	explicit RenderPass(const RenderPassInfo& renderPassInfo);
	~RenderPass();

	vk::RenderPassBeginInfo createRenderPassBeginInfo(const vk::Framebuffer& vulkanFramebuffer, const vk::Extent2D& swapChainExtent) const;
	const vk::RenderPass& getVulkanRenderPass() const;

private:
	vk::AttachmentDescription createMSAAColorAttachmentDescription(const RenderPassInfo& renderPassInfo) const;
	vk::AttachmentDescription createResolveColorAttachmentDescription(const RenderPassInfo& renderPassInfo) const;
	vk::AttachmentDescription createDepthAttachmentDescription(const RenderPassInfo& renderPassInfo) const;
	vk::SubpassDescription createSubpassDescription(const vk::AttachmentReference& MSAAColorAttachmentReference, const vk::AttachmentReference& depthAttachmentReference, const vk::AttachmentReference& resolveColorAttachmentReference) const;
	vk::AttachmentReference createColorAttachmentReference(const uint32_t attachmentIndex) const;
	vk::AttachmentReference createDepthAttachmentReference(const uint32_t attachmentIndex) const;
	vk::SubpassDependency createSubpassDependency() const;
	vk::RenderPassCreateInfo createRenderPassCreateInfo(const std::vector<vk::AttachmentDescription>& attachmentDescriptions, const vk::SubpassDescription& subpassDescription, const vk::SubpassDependency& subpassDependency) const;

	const vk::Device vulkanLogicalDevice;
	vk::RenderPass vulkanRenderPass;
};