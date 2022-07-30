#pragma once

#include "Buffer.h"
#include "ContentBufferCreateInfo.h"

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(const ContentBufferCreateInfo<uint16_t>& contentBufferCreateInfo);
	const size_t getIndexCount() const;

private:
	void createIndexData(const std::vector<uint16_t>& indices, const vk::PhysicalDevice& vulkanPhysicalDevice);
	size_t indexCount;
};