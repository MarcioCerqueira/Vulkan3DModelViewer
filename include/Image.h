#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <stdexcept>
#include <tuple>
#include "MemoryProperties.h"
#include "CommandBuffer.h"
#include "GraphicsQueue.h"
#include "structs/ImageInfo.h"
#include "structs/TransitionLayoutInfo.h"

class Image
{
public:
	Image(const ImageInfo& imageInfo);
	~Image();

	void transitionLayout(const TransitionLayoutInfo& transitionLayoutInfo);
	const vk::Image getVulkanImage() const;
	const vk::BufferImageCopy buildBufferImageCopy() const;

private:
	const vk::ImageCreateInfo buildImageCreateInfo() const;
	const vk::MemoryAllocateInfo buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const;
	const vk::ImageMemoryBarrier buildImageMemoryBarrier(const vk::ImageLayout& oldLayout, const vk::ImageLayout& newLayout) const;
	const vk::ImageSubresourceRange buildImageSubresourceRange() const;
	std::tuple<vk::PipelineStageFlags, vk::PipelineStageFlags> determinePipelineStages(const vk::ImageLayout& oldLayout, const vk::ImageLayout& newLayout) const;
	const CommandBufferPipelineBarrierInfo buildCommandBufferPipelineBarrierInfo(const vk::PipelineStageFlags& srcStage, const vk::PipelineStageFlags& dstStage, const vk::ImageMemoryBarrier& imageMemoryBarrier) const;
	const vk::ImageSubresourceLayers buildImageSubresourceLayers() const;

	const vk::Device vulkanLogicalDevice; 
	const int width;
	const int height;
	vk::Image vulkanImage;
	vk::DeviceMemory vulkanImageMemory;
	
};