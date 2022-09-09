#include "RenderPass.h"

RenderPass::RenderPass(const RenderPassInfo& renderPassInfo) : vulkanLogicalDevice(renderPassInfo.vulkanLogicalDevice)
{
	const vk::AttachmentDescription MSAAColorAttachmentDescription{ createMSAAColorAttachmentDescription(renderPassInfo) };
	const vk::AttachmentDescription resolveColorAttachmentDescription{ createResolveColorAttachmentDescription(renderPassInfo) };
	const vk::AttachmentDescription depthAttachmentDescription{ createDepthAttachmentDescription(renderPassInfo) };
	const vk::AttachmentReference MSAAColorAttachmentReference{ createColorAttachmentReference(0) };
	const vk::AttachmentReference depthAttachmentReference{ createDepthAttachmentReference(1) };
	const vk::AttachmentReference resolveColorAttachmentReference{ createColorAttachmentReference(2) };
	const vk::SubpassDescription subpassDescription{ createSubpassDescription(MSAAColorAttachmentReference, depthAttachmentReference, resolveColorAttachmentReference) };
	const vk::SubpassDependency subpassDependency{ createSubpassDependency() };
	const std::vector<vk::AttachmentDescription> attachmentDescriptions = { 
		MSAAColorAttachmentDescription,
		depthAttachmentDescription,
		resolveColorAttachmentDescription
	};
	const vk::RenderPassCreateInfo renderPassCreateInfo { createRenderPassCreateInfo(attachmentDescriptions, subpassDescription, subpassDependency)};
	vulkanRenderPass = vulkanLogicalDevice.createRenderPass(renderPassCreateInfo);
}

RenderPass::~RenderPass()
{
	vulkanLogicalDevice.destroyRenderPass(vulkanRenderPass);
}

vk::RenderPassCreateInfo RenderPass::createRenderPassCreateInfo(const std::vector<vk::AttachmentDescription>& attachmentDescriptions, const vk::SubpassDescription& subpassDescription, const vk::SubpassDependency& subpassDependency) const
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

vk::AttachmentDescription RenderPass::createMSAAColorAttachmentDescription(const RenderPassInfo& renderPassInfo) const
{
	return vk::AttachmentDescription{
		.format = renderPassInfo.colorImageFormat,
		.samples = renderPassInfo.sampleCount,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
		.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
		.initialLayout = vk::ImageLayout::eUndefined,
		.finalLayout = vk::ImageLayout::eColorAttachmentOptimal
	};
}

vk::AttachmentDescription RenderPass::createResolveColorAttachmentDescription(const RenderPassInfo& renderPassInfo) const
{
	return vk::AttachmentDescription{
		.format = renderPassInfo.colorImageFormat,
		.samples = vk::SampleCountFlagBits::e1,
		.loadOp = vk::AttachmentLoadOp::eDontCare,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
		.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
		.initialLayout = vk::ImageLayout::eUndefined,
		.finalLayout = vk::ImageLayout::ePresentSrcKHR
	};
}

vk::AttachmentDescription RenderPass::createDepthAttachmentDescription(const RenderPassInfo& renderPassInfo) const
{
	return vk::AttachmentDescription{
		.format = renderPassInfo.depthImageFormat,
		.samples = renderPassInfo.sampleCount,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eDontCare,
		.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
		.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
		.initialLayout = vk::ImageLayout::eUndefined,
		.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal
	};
}

vk::SubpassDescription RenderPass::createSubpassDescription(const vk::AttachmentReference& colorAttachmentReference, const vk::AttachmentReference& depthAttachmentReference, const vk::AttachmentReference& resolveColorAttachmentReference) const
{
	return vk::SubpassDescription{
		.pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachmentReference,
		.pResolveAttachments = &resolveColorAttachmentReference,
		.pDepthStencilAttachment = &depthAttachmentReference
	};
}

vk::AttachmentReference RenderPass::createColorAttachmentReference(const uint32_t attachmentIndex) const
{
	return vk::AttachmentReference{
		.attachment = attachmentIndex,
		.layout = vk::ImageLayout::eColorAttachmentOptimal
	};
}

vk::AttachmentReference RenderPass::createDepthAttachmentReference(const uint32_t attachmentIndex) const
{
	return vk::AttachmentReference{
		.attachment = attachmentIndex,
		.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal
	};
}

vk::SubpassDependency RenderPass::createSubpassDependency() const
{
	return vk::SubpassDependency{
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite
	};
}

vk::RenderPassBeginInfo RenderPass::createRenderPassBeginInfo(const vk::Framebuffer& vulkanFramebuffer, const vk::Extent2D& swapChainExtent) const
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

const vk::RenderPass& RenderPass::getVulkanRenderPass() const
{
	return vulkanRenderPass;
}
