#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class GraphicsPipeline
{
public:
	explicit GraphicsPipeline(const vk::Device& vulkanLogicalDevice, const vk::Extent2D& swapChainExtent);

private:
	vk::PipelineVertexInputStateCreateInfo buildPipelineVertexInputStateCreateInfo() const;
	vk::PipelineInputAssemblyStateCreateInfo buildPipelineInputAssemblyStateCreateInfo() const;
	vk::Viewport buildViewport(const vk::Extent2D& swapChainExtent) const;
	vk::Rect2D buildScissor(const vk::Extent2D& swapChainExtent) const;
	vk::PipelineViewportStateCreateInfo buildPipelineViewportStateCreateInfo(const vk::Extent2D& swapChainExtent, const vk::Viewport& viewport, const vk::Rect2D& scissor) const;
	vk::PipelineRasterizationStateCreateInfo buildPipelineRasterizationStateCreateInfo() const;
	vk::PipelineMultisampleStateCreateInfo buildPipelineMultisampleStateCreateInfo() const;
	vk::PipelineColorBlendAttachmentState buildPipelineColorBlendAttachmentState() const;
	vk::PipelineColorBlendStateCreateInfo buildPipelineColorBlendStateCreateInfo(const vk::PipelineColorBlendAttachmentState& pipelineColorBlendAttachmentState) const;
	vk::PipelineDynamicStateCreateInfo buildPipelineDynamicStateCreateInfo() const;
	vk::PipelineLayoutCreateInfo buildPipelineLayoutCreateInfo() const;
	void buildPipelineLayout(const vk::Device& vulkanLogicalDevice, const vk::PipelineLayoutCreateInfo& pipelineLayoutCreateInfo);

	vk::PipelineLayout pipelineLayout;
};