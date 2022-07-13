#include "GraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineCreateInfo& graphicsPipelineCreateInfo) : vulkanLogicalDevice(graphicsPipelineCreateInfo.vulkanLogicalDevice)
{
	const vk::PipelineVertexInputStateCreateInfo vertexInputState{ buildPipelineVertexInputStateCreateInfo() };
	const vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState{ buildPipelineInputAssemblyStateCreateInfo() };
	const vk::Viewport viewport{ buildViewport(graphicsPipelineCreateInfo.swapChainExtent) };
	const vk::Rect2D scissor{ buildScissor(graphicsPipelineCreateInfo.swapChainExtent) };
	const vk::PipelineViewportStateCreateInfo viewportState{ buildPipelineViewportStateCreateInfo(viewport, scissor) };
	const vk::PipelineRasterizationStateCreateInfo rasterizationState{ buildPipelineRasterizationStateCreateInfo() };
	const vk::PipelineMultisampleStateCreateInfo multisampleState{ buildPipelineMultisampleStateCreateInfo() };
	const vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{ buildPipelineColorBlendAttachmentState() };
	const vk::PipelineColorBlendStateCreateInfo  colorBlendState{ buildPipelineColorBlendStateCreateInfo(colorBlendAttachmentState) };
	const vk::PipelineDynamicStateCreateInfo  dynamicState{ buildPipelineDynamicStateCreateInfo() };
	const vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo{ buildPipelineLayoutCreateInfo() };
	pipelineLayout = vulkanLogicalDevice.createPipelineLayout(pipelineLayoutCreateInfo);
	const vk::GraphicsPipelineCreateInfo vulkanGraphicsPipelineCreateInfo{
		.stageCount = 2,
		.pStages = graphicsPipelineCreateInfo.shaderStages.data(),
		.pVertexInputState = &vertexInputState,
		.pInputAssemblyState = &inputAssemblyState,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizationState,
		.pMultisampleState = &multisampleState,
		.pColorBlendState = &colorBlendState,
		.layout = pipelineLayout,
		.renderPass = graphicsPipelineCreateInfo.vulkanRenderPass,
		.subpass = 0,
		.basePipelineHandle = nullptr,
		.basePipelineIndex = -1
	};
	const vk::ResultValue<vk::Pipeline> graphicsPipeline = vulkanLogicalDevice.createGraphicsPipeline({}, vulkanGraphicsPipelineCreateInfo);
	ExceptionChecker::throwExceptionIfVulkanResultIsNotSuccess(graphicsPipeline.result, "Unable to create the graphics pipeline!");
	pipeline = graphicsPipeline.value;
}

GraphicsPipeline::~GraphicsPipeline()
{
	vulkanLogicalDevice.destroyPipeline(pipeline);
	vulkanLogicalDevice.destroyPipelineLayout(pipelineLayout);
}

const vk::PipelineVertexInputStateCreateInfo GraphicsPipeline::buildPipelineVertexInputStateCreateInfo() const
{
	return vk::PipelineVertexInputStateCreateInfo{
		.vertexBindingDescriptionCount = 0,
		.pVertexBindingDescriptions = nullptr,
		.vertexAttributeDescriptionCount = 0,
		.pVertexAttributeDescriptions = nullptr
	};
}

const vk::PipelineInputAssemblyStateCreateInfo GraphicsPipeline::buildPipelineInputAssemblyStateCreateInfo() const
{
	return vk::PipelineInputAssemblyStateCreateInfo{
		.topology = vk::PrimitiveTopology::eTriangleList,
		.primitiveRestartEnable = vk::Bool32(0)
	};
}

const vk::Viewport GraphicsPipeline::buildViewport(const vk::Extent2D& swapChainExtent) const
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

const vk::Rect2D GraphicsPipeline::buildScissor(const vk::Extent2D& swapChainExtent) const
{
	return vk::Rect2D{
		.offset = {0, 0},
		.extent = swapChainExtent
	};
}

const vk::PipelineViewportStateCreateInfo GraphicsPipeline::buildPipelineViewportStateCreateInfo(const vk::Viewport& viewport, const vk::Rect2D& scissor) const
{
	return vk::PipelineViewportStateCreateInfo{
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor
	};
}

const vk::PipelineRasterizationStateCreateInfo GraphicsPipeline::buildPipelineRasterizationStateCreateInfo() const
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

const vk::PipelineMultisampleStateCreateInfo GraphicsPipeline::buildPipelineMultisampleStateCreateInfo() const
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

const vk::PipelineColorBlendAttachmentState GraphicsPipeline::buildPipelineColorBlendAttachmentState() const
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

const vk::PipelineColorBlendStateCreateInfo GraphicsPipeline::buildPipelineColorBlendStateCreateInfo(const vk::PipelineColorBlendAttachmentState& colorBlendAttachmentState) const
{
	const std::array<float, 4> blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f };
	return vk::PipelineColorBlendStateCreateInfo{
		.logicOpEnable = vk::Bool32(0),
		.logicOp = vk::LogicOp::eCopy,
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachmentState,
		.blendConstants = blendConstants
	};
}

const vk::PipelineDynamicStateCreateInfo GraphicsPipeline::buildPipelineDynamicStateCreateInfo() const
{
	const std::vector<vk::DynamicState> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eLineWidth };
	return vk::PipelineDynamicStateCreateInfo{
		.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
		.pDynamicStates = dynamicStates.data()
	};
}

const vk::PipelineLayoutCreateInfo GraphicsPipeline::buildPipelineLayoutCreateInfo() const
{
	return vk::PipelineLayoutCreateInfo{
		.setLayoutCount = 0,
		.pSetLayouts = nullptr,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = nullptr
	};
}

const vk::Pipeline GraphicsPipeline::getVulkanPipeline() const
{
	return pipeline;
}

