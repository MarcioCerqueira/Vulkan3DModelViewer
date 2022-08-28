#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

struct CommandBufferRecordInfo
{
	vk::RenderPassBeginInfo renderPassBeginInfo;
	vk::Pipeline graphicsPipeline;
	vk::PipelineLayout vulkanPipelineLayout;
	vk::Buffer vulkanVertexBuffer;
	vk::Buffer vulkanIndexBuffer;
	vk::DescriptorSet vulkanDescriptorSet;
	unsigned int frameIndex;
	size_t indexCount;
	vk::IndexType indexType;
	vk::Extent2D swapChainExtent;
	uint32_t firstDescriptorSet = 0;
	uint32_t descriptorSetCount = 1;
	uint32_t descriptorDynamicOffsetCount = 0;
	uint32_t* descriptorDynamicOffset = nullptr;
	uint32_t firstBinding = 0;
	uint32_t bindingCount = 1;
	uint32_t instanceCount = 1;
	uint32_t firstIndex = 0;
	uint32_t firstInstance = 0;
	uint32_t vertexOffset = 0;
	vk::DeviceSize offset = 0;
};