#include "GraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineCreateInfo& graphicsPipelineCreateInfo) : vulkanLogicalDevice(graphicsPipelineCreateInfo.vulkanLogicalDevice)
{
	const vk::VertexInputBindingDescription vertexBindingDescription{ Vertex::getBindingDescription() };
	const std::array<vk::VertexInputAttributeDescription, 3> vertexAttributeDescriptions{ Vertex::getAttributeDescriptions() };
	const vk::PipelineVertexInputStateCreateInfo vertexInputState{ buildPipelineVertexInputStateCreateInfo(vertexBindingDescription, vertexAttributeDescriptions) };
	const vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState{ buildPipelineInputAssemblyStateCreateInfo() };
	const vk::PipelineViewportStateCreateInfo viewportState{ buildPipelineViewportStateCreateInfo() };
	const vk::PipelineRasterizationStateCreateInfo rasterizationState{ buildPipelineRasterizationStateCreateInfo() };
	const vk::PipelineMultisampleStateCreateInfo multisampleState{ buildPipelineMultisampleStateCreateInfo(graphicsPipelineCreateInfo.sampleCount) };
	const vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{ buildPipelineColorBlendAttachmentState() };
	const vk::PipelineColorBlendStateCreateInfo  colorBlendState{ buildPipelineColorBlendStateCreateInfo(colorBlendAttachmentState) };
	const std::vector<vk::DynamicState> dynamicStates = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
	const vk::PipelineDynamicStateCreateInfo  dynamicState{ buildPipelineDynamicStateCreateInfo(dynamicStates) };
	const vk::PipelineDepthStencilStateCreateInfo depthStencilState{ buildPipelineDepthStencilStateCreateInfo() };
	const vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo{ buildPipelineLayoutCreateInfo(graphicsPipelineCreateInfo.vulkanDescriptorSetLayout) };
	pipelineLayout = vulkanLogicalDevice.createPipelineLayout(pipelineLayoutCreateInfo);
	const vk::GraphicsPipelineCreateInfo vulkanGraphicsPipelineCreateInfo{
		.stageCount = 2,
		.pStages = graphicsPipelineCreateInfo.shaderStages.data(),
		.pVertexInputState = &vertexInputState,
		.pInputAssemblyState = &inputAssemblyState,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizationState,
		.pMultisampleState = &multisampleState,
		.pDepthStencilState = &depthStencilState,
		.pColorBlendState = &colorBlendState,
		.pDynamicState = &dynamicState,
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

const vk::PipelineVertexInputStateCreateInfo GraphicsPipeline::buildPipelineVertexInputStateCreateInfo(const vk::VertexInputBindingDescription& vertexBindingDescription, const std::array<vk::VertexInputAttributeDescription, 3>& vertexAttributeDescriptions) const
{	
	return vk::PipelineVertexInputStateCreateInfo{
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &vertexBindingDescription,
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributeDescriptions.size()),
		.pVertexAttributeDescriptions = vertexAttributeDescriptions.data()
	};
}

const vk::PipelineInputAssemblyStateCreateInfo GraphicsPipeline::buildPipelineInputAssemblyStateCreateInfo() const
{
	return vk::PipelineInputAssemblyStateCreateInfo{
		.topology = vk::PrimitiveTopology::eTriangleList,
		.primitiveRestartEnable = vk::Bool32(0)
	};
}

const vk::PipelineViewportStateCreateInfo GraphicsPipeline::buildPipelineViewportStateCreateInfo() const
{
	return vk::PipelineViewportStateCreateInfo{
		.viewportCount = 1,
		.scissorCount = 1,
	};
}

const vk::PipelineRasterizationStateCreateInfo GraphicsPipeline::buildPipelineRasterizationStateCreateInfo() const
{
	return vk::PipelineRasterizationStateCreateInfo{
		.depthClampEnable = vk::Bool32(0),
		.rasterizerDiscardEnable = vk::Bool32(0),
		.polygonMode = vk::PolygonMode::eFill,
		.cullMode = vk::CullModeFlagBits::eBack,
		.frontFace = vk::FrontFace::eCounterClockwise,
		.depthBiasEnable = vk::Bool32(0),
		.depthBiasConstantFactor = 0.0f,
		.depthBiasClamp = 0.0f,
		.depthBiasSlopeFactor = 0.0f,
		.lineWidth = 1.0f,
	};
}

const vk::PipelineMultisampleStateCreateInfo GraphicsPipeline::buildPipelineMultisampleStateCreateInfo(const vk::SampleCountFlagBits& sampleCount) const
{
	return vk::PipelineMultisampleStateCreateInfo{
		.rasterizationSamples = sampleCount,
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

const vk::PipelineDynamicStateCreateInfo GraphicsPipeline::buildPipelineDynamicStateCreateInfo(const std::vector<vk::DynamicState>& dynamicStates) const
{
	return vk::PipelineDynamicStateCreateInfo{
		.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
		.pDynamicStates = dynamicStates.data()
	};
}

const vk::PipelineDepthStencilStateCreateInfo GraphicsPipeline::buildPipelineDepthStencilStateCreateInfo() const
{
	return vk::PipelineDepthStencilStateCreateInfo{
		.depthTestEnable = vk::Bool32(1),
		.depthWriteEnable = vk::Bool32(1),
		.depthCompareOp = vk::CompareOp::eLess,
		.depthBoundsTestEnable = vk::Bool32(0),
		.minDepthBounds = 0.0f,
		.maxDepthBounds = 1.0f
	};
}

const vk::PipelineLayoutCreateInfo GraphicsPipeline::buildPipelineLayoutCreateInfo(const vk::DescriptorSetLayout& vulkanDescriptorSetLayout) const
{
	return vk::PipelineLayoutCreateInfo{
		.setLayoutCount = 1,
		.pSetLayouts = &vulkanDescriptorSetLayout,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = nullptr
	};
}

const vk::Pipeline GraphicsPipeline::getVulkanPipeline() const
{
	return pipeline;
}

const vk::PipelineLayout GraphicsPipeline::getVulkanPipelineLayout() const
{
	return pipelineLayout;
}
