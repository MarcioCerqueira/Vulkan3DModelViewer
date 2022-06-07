#include "RenderPass.h"

RenderPass::RenderPass(const vk::Device& vulkanLogicalDevice, const vk::SurfaceFormatKHR& swapChainSurfaceFormat)
{
	const vk::AttachmentDescription attachmentDescription = createAttachmentDescription(swapChainSurfaceFormat);
	const vk::AttachmentReference attachmentReference = createAttachmentReference();
	const vk::SubpassDescription subpassDescription = createSubpassDescription(attachmentReference);
	const vk::RenderPassCreateInfo renderPassCreateInfo = createRenderPassCreateInfo(attachmentDescription, subpassDescription);
	vulkanRenderPass = vulkanLogicalDevice.createRenderPass(renderPassCreateInfo);
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

vk::AttachmentReference RenderPass::createAttachmentReference() const
{
	return vk::AttachmentReference{
		.attachment = 0,
		.layout = vk::ImageLayout::eColorAttachmentOptimal
	};
}

vk::SubpassDescription RenderPass::createSubpassDescription(const vk::AttachmentReference& attachmentReference) const
{
	return vk::SubpassDescription{
		.pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
		.colorAttachmentCount = 1,
		.pColorAttachments = &attachmentReference
	};
}

vk::RenderPassCreateInfo RenderPass::createRenderPassCreateInfo(const vk::AttachmentDescription& attachmentDescription, const vk::SubpassDescription& subpassDescription)
{
	return vk::RenderPassCreateInfo{
		.sType = vk::StructureType::eRenderPassCreateInfo,
		.attachmentCount = 1,
		.pAttachments = &attachmentDescription,
		.subpassCount = 1,
		.pSubpasses = &subpassDescription
	};
}