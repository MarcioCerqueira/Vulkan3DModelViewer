#include "RenderPass.h"

RenderPass::RenderPass(const vk::Device& vulkanLogicalDevice, const vk::SurfaceFormatKHR& swapChainSurfaceFormat) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::RenderPassCreateInfo renderPassCreateInfo{ createRenderPassCreateInfo(swapChainSurfaceFormat) };
	vulkanRenderPass = vulkanLogicalDevice.createRenderPass(renderPassCreateInfo);
}

RenderPass::~RenderPass()
{
	vulkanLogicalDevice.destroyRenderPass(vulkanRenderPass);
}

vk::RenderPass RenderPass::getVulkanRenderPass() const noexcept
{
	return vulkanRenderPass;
}

vk::RenderPassCreateInfo RenderPass::createRenderPassCreateInfo(const vk::SurfaceFormatKHR& swapChainSurfaceFormat) const
{
	const vk::AttachmentDescription attachmentDescription{ createAttachmentDescription(swapChainSurfaceFormat) };
	const vk::SubpassDescription subpassDescription{ createSubpassDescription() };
	return vk::RenderPassCreateInfo{
		.attachmentCount = 1,
		.pAttachments = &attachmentDescription,
		.subpassCount = 1,
		.pSubpasses = &subpassDescription
	};
}

vk::AttachmentDescription RenderPass::createAttachmentDescription(const vk::SurfaceFormatKHR& swapChainSurfaceFormat) const
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

vk::SubpassDescription RenderPass::createSubpassDescription() const
{
	const vk::AttachmentReference attachmentReference{ createAttachmentReference() };
	return vk::SubpassDescription{
		.pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
		.colorAttachmentCount = 1,
		.pColorAttachments = &attachmentReference
	};
}

vk::AttachmentReference RenderPass::createAttachmentReference() const
{
	return vk::AttachmentReference{
		.attachment = 0,
		.layout = vk::ImageLayout::eColorAttachmentOptimal
	};
}
