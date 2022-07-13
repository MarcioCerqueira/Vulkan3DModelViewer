#include "RenderPass.h"

RenderPass::RenderPass(const vk::Device& vulkanLogicalDevice, const vk::SurfaceFormatKHR& swapChainSurfaceFormat) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::AttachmentDescription attachmentDescription{ createAttachmentDescription(swapChainSurfaceFormat) };
	const vk::AttachmentReference attachmentReference{ createAttachmentReference() };
	const vk::SubpassDescription subpassDescription{ createSubpassDescription(attachmentReference) };
	const vk::SubpassDependency subpassDependency{ createSubpassDependency() };
	const vk::RenderPassCreateInfo renderPassInfo { createRenderPassCreateInfo(attachmentDescription, subpassDescription, subpassDependency)};
	vulkanRenderPass = vulkanLogicalDevice.createRenderPass(renderPassInfo);
}

RenderPass::~RenderPass()
{
	vulkanLogicalDevice.destroyRenderPass(vulkanRenderPass);
}

const vk::RenderPassCreateInfo RenderPass::createRenderPassCreateInfo(const vk::AttachmentDescription& attachmentDescription, const vk::SubpassDescription& subpassDescription, const vk::SubpassDependency& subpassDependency) const
{
	return vk::RenderPassCreateInfo{
		.attachmentCount = 1,
		.pAttachments = &attachmentDescription,
		.subpassCount = 1,
		.pSubpasses = &subpassDescription,
		.dependencyCount = 1,
		.pDependencies = &subpassDependency
	};
}

const vk::AttachmentDescription RenderPass::createAttachmentDescription(const vk::SurfaceFormatKHR& swapChainSurfaceFormat) const
{
	return vk::AttachmentDescription{
		.format = swapChainSurfaceFormat.format,
		.samples = vk::SampleCountFlagBits::e1,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
		.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
		.initialLayout = vk::ImageLayout::eUndefined,
		.finalLayout = vk::ImageLayout::ePresentSrcKHR
	};
}

const vk::SubpassDescription RenderPass::createSubpassDescription(const vk::AttachmentReference& attachmentReference) const
{
	return vk::SubpassDescription{
		.pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
		.colorAttachmentCount = 1,
		.pColorAttachments = &attachmentReference
	};
}

const vk::AttachmentReference RenderPass::createAttachmentReference() const
{
	return vk::AttachmentReference{
		.attachment = 0,
		.layout = vk::ImageLayout::eColorAttachmentOptimal
	};
}

const vk::SubpassDependency RenderPass::createSubpassDependency() const
{
	return vk::SubpassDependency{
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
		.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
		.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite
	};
}

const vk::RenderPassBeginInfo RenderPass::createRenderPassBeginInfo(const vk::Framebuffer& vulkanFramebuffer, const vk::Extent2D& swapChainExtent) const
{
	vk::ClearValue clearColor{
		.color = vk::ClearColorValue(std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f })
	};
	vk::Rect2D renderArea{
		.offset = {0, 0},
		.extent = swapChainExtent
	};
	return vk::RenderPassBeginInfo{
		.renderPass = vulkanRenderPass,
		.framebuffer = vulkanFramebuffer,
		.renderArea = renderArea,
		.clearValueCount = 1,
		.pClearValues = &clearColor
	};
}

const vk::RenderPass RenderPass::getVulkanRenderPass() const
{
	return vulkanRenderPass;
}
