#pragma once

#include "StagingBuffer.h"
#include "Vertex.h"
#include "structs/ContentBufferCreateInfo.h"

class VertexBuffer
{
public:
	VertexBuffer(const ContentBufferCreateInfo<Vertex>& contentBufferCreateInfo);
	size_t getVertexCount() const;
	const vk::Buffer& getVulkanBuffer() const;

private:
	void createVertexData(const std::vector<Vertex>& vertices, const PhysicalDeviceProperties& physicalDeviceProperties);
	size_t vertexCount;
	Buffer buffer;
	StagingBuffer stagingBuffer;
};
