#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "GraphicsPipelineCreateInfo.h"

class GraphicsPipeline
{
public:
	GraphicsPipeline(const GraphicsPipelineCreateInfo& graphicsPipelineCreateInfo);
	~GraphicsPipeline();

private:
	vk::PipelineVertexInputStateCreateInfo buildPipelineVertexInputStateCreateInfo() const;
	vk::PipelineInputAssemblyStateCreateInfo buildPipelineInputAssemblyStateCreateInfo() const;
	vk::PipelineViewportStateCreateInfo buildPipelineViewportStateCreateInfo(const vk::Extent2D& swapChainExtent) const;
	vk::Viewport buildViewport(const vk::Extent2D& swapChainExtent) const;
	vk::Rect2D buildScissor(const vk::Extent2D& swapChainExtent) const;
	vk::PipelineRasterizationStateCreateInfo buildPipelineRasterizationStateCreateInfo() const;
	vk::PipelineMultisampleStateCreateInfo buildPipelineMultisampleStateCreateInfo() const;
	vk::PipelineColorBlendStateCreateInfo buildPipelineColorBlendStateCreateInfo() const;
	vk::PipelineColorBlendAttachmentState buildPipelineColorBlendAttachmentState() const;
	vk::PipelineDynamicStateCreateInfo buildPipelineDynamicStateCreateInfo() const;
	vk::PipelineLayoutCreateInfo buildPipelineLayoutCreateInfo() const;
	void buildPipelineLayout(const vk::Device& vulkanLogicalDevice, const vk::PipelineLayoutCreateInfo& pipelineLayoutCreateInfo);
	
	const vk::Device vulkanLogicalDevice;
	vk::PipelineLayout pipelineLayout;
	vk::Pipeline pipeline;
};