#pragma once

#include "StagingBuffer.h"
#include "structs/ContentBufferCreateInfo.h"

class IndexBuffer
{
public:
	IndexBuffer(const ContentBufferCreateInfo<uint16_t>& contentBufferCreateInfo);
	const size_t getIndexCount() const;
	const vk::Buffer getVulkanBuffer() const;

private:
	void createIndexData(const std::vector<uint16_t>& indices, const PhysicalDeviceProperties& physicalDeviceProperties);
	size_t indexCount;
	Buffer buffer;
	StagingBuffer stagingBuffer;
};