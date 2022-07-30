#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "structs/GraphicsPipelineCreateInfo.h"
#include "ExceptionChecker.h"
#include "Vertex.h"

class GraphicsPipeline
{
public:
	GraphicsPipeline(const GraphicsPipelineCreateInfo& graphicsPipelineCreateInfo);
	~GraphicsPipeline();

	const vk::Pipeline getVulkanPipeline() const;

private:
	const vk::PipelineVertexInputStateCreateInfo buildPipelineVertexInputStateCreateInfo(const vk::VertexInputBindingDescription& vertexBindingDescription, const std::array<vk::VertexInputAttributeDescription, 2>&  vertexAttributeDescriptions) const;
	const vk::PipelineInputAssemblyStateCreateInfo buildPipelineInputAssemblyStateCreateInfo() const;
	const vk::Viewport buildViewport(const vk::Extent2D& swapChainExtent) const;
	const vk::Rect2D buildScissor(const vk::Extent2D& swapChainExtent) const;
	const vk::PipelineViewportStateCreateInfo buildPipelineViewportStateCreateInfo(const vk::Viewport& viewport, const vk::Rect2D& scissor) const;
	const vk::PipelineRasterizationStateCreateInfo buildPipelineRasterizationStateCreateInfo() const;
	const vk::PipelineMultisampleStateCreateInfo buildPipelineMultisampleStateCreateInfo() const;
	const vk::PipelineColorBlendAttachmentState buildPipelineColorBlendAttachmentState() const;
	const vk::PipelineColorBlendStateCreateInfo buildPipelineColorBlendStateCreateInfo(const vk::PipelineColorBlendAttachmentState& colorBlendAttachmentState) const;
	const vk::PipelineDynamicStateCreateInfo buildPipelineDynamicStateCreateInfo() const;
	const vk::PipelineLayoutCreateInfo buildPipelineLayoutCreateInfo() const;
	
	const vk::Device vulkanLogicalDevice;
	vk::PipelineLayout pipelineLayout;
	vk::Pipeline pipeline;
};