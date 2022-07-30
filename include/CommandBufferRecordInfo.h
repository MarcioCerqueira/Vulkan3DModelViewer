#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

struct CommandBufferRecordInfo
{
	vk::RenderPassBeginInfo renderPassBeginInfo;
	vk::Pipeline graphicsPipeline;
	vk::Buffer vulkanVertexBuffer;
	vk::Buffer vulkanIndexBuffer;
	unsigned int frameIndex;
	size_t indexCount;
	vk::IndexType indexType;
	uint32_t firstBinding = 0;
	uint32_t bindingCount = 1;
	uint32_t instanceCount = 1;
	uint32_t firstIndex = 0;
	uint32_t firstInstance = 0;
	uint32_t vertexOffset = 0;
	vk::DeviceSize offset = 0;
};