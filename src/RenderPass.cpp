#include "RenderPass.h"

RenderPass::RenderPass(const vk::Device& vulkanLogicalDevice, const vk::Format& colorImageFormat, const vk::Format& depthImageFormat) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::AttachmentDescription colorAttachmentDescription{ createColorAttachmentDescription(colorImageFormat) };
	const vk::AttachmentDescription depthAttachmentDescription{ createDepthAttachmentDescription(depthImageFormat) };
	const vk::AttachmentReference colorAttachmentReference{ createColorAttachmentReference() };
	const vk::AttachmentReference depthAttachmentReference{ createDepthAttachmentReference() };
	const vk::SubpassDescription subpassDescription{ createSubpassDescription(colorAttachmentReference, depthAttachmentReference) };
	const vk::SubpassDependency subpassDependency{ createSubpassDependency() };
	const std::vector<vk::AttachmentDescription> attachmentDescriptions = { colorAttachmentDescription,
		depthAttachmentDescription
	};
	const vk::RenderPassCreateInfo renderPassInfo { createRenderPassCreateInfo(attachmentDescriptions, subpassDescription, subpassDependency)};
	vulkanRenderPass = vulkanLogicalDevice.createRenderPass(renderPassInfo);
}

RenderPass::~RenderPass()
{
	vulkanLogicalDevice.destroyRenderPass(vulkanRenderPass);
}

const vk::RenderPassCreateInfo RenderPass::createRenderPassCreateInfo(const std::vector<vk::AttachmentDescription>& attachmentDescriptions, const vk::SubpassDescription& subpassDescription, const vk::SubpassDependency& subpassDependency) const
{
	return vk::RenderPassCreateInfo{
		.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size()),
		.pAttachments = attachmentDescriptions.data(),
		.subpassCount = 1,
		.pSubpasses = &subpassDescription,
		.dependencyCount = 1,
		.pDependencies = &subpassDependency
	};
}

const vk::AttachmentDescription RenderPass::createColorAttachmentDescription(const vk::Format& colorImageFormat) const
{
	return vk::AttachmentDescription{
		.format = colorImageFormat,
		.samples = vk::SampleCountFlagBits::e1,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
		.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
		.initialLayout = vk::ImageLayout::eUndefined,
		.finalLayout = vk::ImageLayout::ePresentSrcKHR
	};
}

const vk::AttachmentDescription RenderPass::createDepthAttachmentDescription(const vk::Format& depthImageFormat) const
{
	return vk::AttachmentDescription{
		.format = depthImageFormat,
		.samples = vk::SampleCountFlagBits::e1,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eDontCare,
		.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
		.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
		.initialLayout = vk::ImageLayout::eUndefined,
		.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal
	};
}

const vk::SubpassDescription RenderPass::createSubpassDescription(const vk::AttachmentReference& colorAttachmentReference, const vk::AttachmentReference& depthAttachmentReference) const
{
	return vk::SubpassDescription{
		.pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachmentReference,
		.pDepthStencilAttachment = &depthAttachmentReference
	};
}

const vk::AttachmentReference RenderPass::createColorAttachmentReference() const
{
	return vk::AttachmentReference{
		.attachment = 0,
		.layout = vk::ImageLayout::eColorAttachmentOptimal
	};
}

const vk::AttachmentReference RenderPass::createDepthAttachmentReference() const
{
	return vk::AttachmentReference{
		.attachment = 1,
		.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal
	};
}

const vk::SubpassDependency RenderPass::createSubpassDependency() const
{
	return vk::SubpassDependency{
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite
	};
}

const vk::RenderPassBeginInfo RenderPass::createRenderPassBeginInfo(const vk::Framebuffer& vulkanFramebuffer, const vk::Extent2D& swapChainExtent) const
{
	std::array<vk::ClearValue, 2> clearValues{};
	clearValues[0].color = vk::ClearColorValue(std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f });
	clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0);
	vk::Rect2D renderArea{
		.offset = {0, 0},
		.extent = swapChainExtent
	};
	return vk::RenderPassBeginInfo{
		.renderPass = vulkanRenderPass,
		.framebuffer = vulkanFramebuffer,
		.renderArea = renderArea,
		.clearValueCount = static_cast<uint32_t>(clearValues.size()),
		.pClearValues = clearValues.data()
	};
}

const vk::RenderPass RenderPass::getVulkanRenderPass() const
{
	return vulkanRenderPass;
}
