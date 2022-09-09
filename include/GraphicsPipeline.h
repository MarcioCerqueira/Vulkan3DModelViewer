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

	const vk::Pipeline& getVulkanPipeline() const;
	const vk::PipelineLayout& getVulkanPipelineLayout() const;

private:
	vk::PipelineVertexInputStateCreateInfo buildPipelineVertexInputStateCreateInfo(const vk::VertexInputBindingDescription& vertexBindingDescription, const std::array<vk::VertexInputAttributeDescription, 3>&  vertexAttributeDescriptions) const;
	vk::PipelineInputAssemblyStateCreateInfo buildPipelineInputAssemblyStateCreateInfo() const;
	vk::PipelineViewportStateCreateInfo buildPipelineViewportStateCreateInfo() const;
	vk::PipelineRasterizationStateCreateInfo buildPipelineRasterizationStateCreateInfo() const;
	vk::PipelineMultisampleStateCreateInfo buildPipelineMultisampleStateCreateInfo(const vk::SampleCountFlagBits& sampleCount) const;
	vk::PipelineColorBlendAttachmentState buildPipelineColorBlendAttachmentState() const;
	vk::PipelineColorBlendStateCreateInfo buildPipelineColorBlendStateCreateInfo(const vk::PipelineColorBlendAttachmentState& colorBlendAttachmentState) const;
	vk::PipelineDynamicStateCreateInfo buildPipelineDynamicStateCreateInfo(const std::vector<vk::DynamicState>& dynamicStates) const;
	vk::PipelineDepthStencilStateCreateInfo buildPipelineDepthStencilStateCreateInfo() const;
	vk::PipelineLayoutCreateInfo buildPipelineLayoutCreateInfo(const vk::DescriptorSetLayout& vulkanDescriptorSetLayout) const;
	
	const vk::Device vulkanLogicalDevice;
	vk::PipelineLayout pipelineLayout;
	vk::Pipeline pipeline;
};