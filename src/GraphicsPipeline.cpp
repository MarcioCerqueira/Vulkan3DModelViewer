#include "GraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline(const vk::Device& vulkanLogicalDevice, const vk::Extent2D& swapChainExtent, const std::vector<vk::PipelineShaderStageCreateInfo>& shaderStages, vk::RenderPass vulkanRenderPass) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo{ buildPipelineLayoutCreateInfo() };
	vk::PipelineVertexInputStateCreateInfo vertexInputState{ buildPipelineVertexInputStateCreateInfo() };
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState{ buildPipelineInputAssemblyStateCreateInfo() };
	vk::Viewport viewport{ buildViewport(swapChainExtent) };
	vk::Rect2D scissor{ buildScissor(swapChainExtent) };
	vk::PipelineViewportStateCreateInfo _viewportState{ buildPipelineViewportStateCreateInfo(swapChainExtent, viewport, scissor) };
	vk::PipelineRasterizationStateCreateInfo rasterizationState{ buildPipelineRasterizationStateCreateInfo() };
	vk::PipelineMultisampleStateCreateInfo multisampleState{ buildPipelineMultisampleStateCreateInfo() };
	vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{ buildPipelineColorBlendAttachmentState() };
	vk::PipelineColorBlendStateCreateInfo  colorBlendState{ buildPipelineColorBlendStateCreateInfo(colorBlendAttachmentState) };
	vk::PipelineDynamicStateCreateInfo  dynamicState{ buildPipelineDynamicStateCreateInfo() };
	pipelineLayout = vulkanLogicalDevice.createPipelineLayout(pipelineLayoutCreateInfo);
	vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo{
		.stageCount = 2,
		.pStages = shaderStages.data(),
		.pVertexInputState = &vertexInputState,
		.pInputAssemblyState = &inputAssemblyState,
		.pViewportState = &_viewportState,
		.pRasterizationState = &rasterizationState,
		.pMultisampleState = &multisampleState,
		.pColorBlendState = &colorBlendState,
		.layout = pipelineLayout,
		.renderPass = vulkanRenderPass,
		.subpass = 0,
		.basePipelineHandle = nullptr,
		.basePipelineIndex = -1
	};
	vk::ResultValue<vk::Pipeline> graphicsPipeline = vulkanLogicalDevice.createGraphicsPipeline({}, graphicsPipelineCreateInfo);
	if (graphicsPipeline.result == vk::Result::eSuccess)
	{
		pipeline = graphicsPipeline.value;
	}
	else
	{
		throw std::runtime_error("Error! Unable to create the graphics pipeline");
	}
}

GraphicsPipeline::~GraphicsPipeline()
{
	vulkanLogicalDevice.destroyPipeline(pipeline);
	vulkanLogicalDevice.destroyPipelineLayout(pipelineLayout);
}

vk::GraphicsPipelineCreateInfo GraphicsPipeline::buildGraphicsPipelineCreateInfo(const vk::Extent2D& swapChainExtent, const std::vector<vk::PipelineShaderStageCreateInfo>& shaderStages, vk::RenderPass vulkanRenderPass) const
{
	vk::PipelineVertexInputStateCreateInfo vertexInputState{ buildPipelineVertexInputStateCreateInfo() };
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState{ buildPipelineInputAssemblyStateCreateInfo() };
	vk::Viewport viewport{ buildViewport(swapChainExtent) };
	vk::Rect2D scissor{ buildScissor(swapChainExtent) };
	vk::PipelineViewportStateCreateInfo viewportState{ buildPipelineViewportStateCreateInfo(swapChainExtent, viewport, scissor)};
	vk::PipelineRasterizationStateCreateInfo rasterizationState{ buildPipelineRasterizationStateCreateInfo() };
	vk::PipelineMultisampleStateCreateInfo multisampleState{ buildPipelineMultisampleStateCreateInfo() };
	vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{ buildPipelineColorBlendAttachmentState() };
	vk::PipelineColorBlendStateCreateInfo  colorBlendState{ buildPipelineColorBlendStateCreateInfo(colorBlendAttachmentState) };
	vk::PipelineDynamicStateCreateInfo  dynamicState{ buildPipelineDynamicStateCreateInfo() };
	return vk::GraphicsPipelineCreateInfo{
		.stageCount = 2,
		.pStages = shaderStages.data(),
		.pVertexInputState = &vertexInputState,
		.pInputAssemblyState = &inputAssemblyState,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizationState,
		.pMultisampleState = &multisampleState,
		.pColorBlendState = &colorBlendState,
		.pDynamicState = &dynamicState,
		.layout = pipelineLayout,
		.renderPass = vulkanRenderPass,
		.subpass = 0,
		.basePipelineHandle = nullptr,
		.basePipelineIndex = -1
	};
}

vk::PipelineVertexInputStateCreateInfo GraphicsPipeline::buildPipelineVertexInputStateCreateInfo() const
{
	return vk::PipelineVertexInputStateCreateInfo{
		.vertexBindingDescriptionCount = 0,
		.pVertexBindingDescriptions = nullptr,
		.vertexAttributeDescriptionCount = 0,
		.pVertexAttributeDescriptions = nullptr
	};
}

vk::PipelineInputAssemblyStateCreateInfo GraphicsPipeline::buildPipelineInputAssemblyStateCreateInfo() const
{
	return vk::PipelineInputAssemblyStateCreateInfo{
		.topology = vk::PrimitiveTopology::eTriangleList,
		.primitiveRestartEnable = vk::Bool32(0)
	};
}

vk::Viewport GraphicsPipeline::buildViewport(const vk::Extent2D& swapChainExtent) const
{
	return vk::Viewport{
		.x = 0.0f,
		.y = 0.0f,
		.width = static_cast<float>(swapChainExtent.width),
		.height = static_cast<float>(swapChainExtent.height),
		.minDepth = 0.0f,
		.maxDepth = 1.0f
	};
}

vk::Rect2D GraphicsPipeline::buildScissor(const vk::Extent2D& swapChainExtent) const
{
	return vk::Rect2D{
		.offset = {0, 0},
		.extent = swapChainExtent
	};
}

vk::PipelineViewportStateCreateInfo GraphicsPipeline::buildPipelineViewportStateCreateInfo(const vk::Extent2D& swapChainExtent, const vk::Viewport& viewport, const vk::Rect2D& scissor) const
{
	return vk::PipelineViewportStateCreateInfo{
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor
	};
}


vk::PipelineRasterizationStateCreateInfo GraphicsPipeline::buildPipelineRasterizationStateCreateInfo() const
{
	return vk::PipelineRasterizationStateCreateInfo{
		.depthClampEnable = vk::Bool32(0),
		.rasterizerDiscardEnable = vk::Bool32(0),
		.polygonMode = vk::PolygonMode::eFill,
		.cullMode = vk::CullModeFlagBits::eBack,
		.frontFace = vk::FrontFace::eClockwise,
		.depthBiasEnable = vk::Bool32(0),
		.depthBiasConstantFactor = 0.0f,
		.depthBiasClamp = 0.0f,
		.depthBiasSlopeFactor = 0.0f,
		.lineWidth = 1.0f,
	};
}

vk::PipelineMultisampleStateCreateInfo GraphicsPipeline::buildPipelineMultisampleStateCreateInfo() const
{
	return vk::PipelineMultisampleStateCreateInfo{
		.rasterizationSamples = vk::SampleCountFlagBits::e1,
		.sampleShadingEnable = vk::Bool32(0),
		.minSampleShading = 1.0f,
		.pSampleMask = nullptr,
		.alphaToCoverageEnable = vk::Bool32(0),
		.alphaToOneEnable = vk::Bool32(0)
	};
}

vk::PipelineColorBlendAttachmentState GraphicsPipeline::buildPipelineColorBlendAttachmentState() const
{
	return vk::PipelineColorBlendAttachmentState{
		.blendEnable = vk::Bool32(0),
		.srcColorBlendFactor = vk::BlendFactor::eOne,
		.dstColorBlendFactor = vk::BlendFactor::eZero,
		.colorBlendOp = vk::BlendOp::eAdd,
		.srcAlphaBlendFactor = vk::BlendFactor::eOne,
		.dstAlphaBlendFactor = vk::BlendFactor::eZero,
		.alphaBlendOp = vk::BlendOp::eAdd,
		.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB
		| vk::ColorComponentFlagBits::eA
	};
}

vk::PipelineColorBlendStateCreateInfo GraphicsPipeline::buildPipelineColorBlendStateCreateInfo(const vk::PipelineColorBlendAttachmentState& pipelineColorBlendAttachmentState) const
{
	const std::array<float, 4> blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f };
	return vk::PipelineColorBlendStateCreateInfo{
		.logicOpEnable = vk::Bool32(0),
		.logicOp = vk::LogicOp::eCopy,
		.attachmentCount = 1,
		.pAttachments = &pipelineColorBlendAttachmentState,
		.blendConstants = blendConstants
	};
}

vk::PipelineDynamicStateCreateInfo GraphicsPipeline::buildPipelineDynamicStateCreateInfo() const
{
	const std::vector<vk::DynamicState> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eLineWidth };
	return vk::PipelineDynamicStateCreateInfo{
		.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
		.pDynamicStates = dynamicStates.data()
	};
}

vk::PipelineLayoutCreateInfo GraphicsPipeline::buildPipelineLayoutCreateInfo() const
{
	return vk::PipelineLayoutCreateInfo{
		.setLayoutCount = 0,
		.pSetLayouts = nullptr,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = nullptr
	};
}

