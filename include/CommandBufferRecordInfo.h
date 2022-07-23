#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

struct CommandBufferRecordInfo
{
	vk::RenderPassBeginInfo renderPassBeginInfo;
	vk::Pipeline graphicsPipeline;
	vk::Buffer vulkanVertexBuffer;
	unsigned int frameIndex;
	size_t vertexCount;
};