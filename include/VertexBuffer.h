#pragma once

#include "Buffer.h"
#include "Vertex.h"
#include "structs/ContentBufferCreateInfo.h"

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(const ContentBufferCreateInfo<Vertex>& contentBufferCreateInfo);
	const size_t getVertexCount() const;

private:
	void createVertexData(const std::vector<Vertex>& vertices, const vk::PhysicalDevice& vulkanPhysicalDevice);
	size_t vertexCount;
};
