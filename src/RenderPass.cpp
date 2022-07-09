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

const vk::RenderPassCreateInfo RenderPass::createRenderPassCreateInfo(const vk::SurfaceFormatKHR& swapChainSurfaceFormat) const
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

const vk::SubpassDescription RenderPass::createSubpassDescription() const
{
	const vk::AttachmentReference attachmentReference{ createAttachmentReference() };
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
