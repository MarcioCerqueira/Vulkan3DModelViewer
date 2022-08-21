#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>

struct GraphicsPipelineCreateInfo
{
	vk::Device vulkanLogicalDevice;
	vk::Extent2D swapChainExtent;
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
	vk::RenderPass vulkanRenderPass;
	vk::DescriptorSetLayout vulkanDescriptorSetLayout;
	vk::SampleCountFlagBits sampleCount;
};