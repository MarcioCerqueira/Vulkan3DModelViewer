#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <stdexcept>
#include <tuple>
#include "MemoryProperties.h"
#include "CommandBuffer.h"
#include "ImageView.h"
#include "Sampler.h"
#include "structs/ImageInfo.h"

class Image
{
public:
	Image(const ImageInfo& imageInfo);
	~Image();

	void transitionLayout(const vk::ImageLayout& oldLayout, const vk::ImageLayout& newLayout, std::shared_ptr<CommandBuffer>& commandBuffers);
	void createImageView();
	const vk::Image getVulkanImage() const;
	const vk::ImageView getVulkanImageView() const;
	const vk::Sampler getVulkanSampler() const;
	const vk::ImageLayout getImageLayout() const;
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
	const vk::Format format;
	vk::Image vulkanImage;
	std::unique_ptr<ImageView> imageView;
	vk::DeviceMemory vulkanImageMemory;
	const Sampler sampler;
	vk::ImageLayout imageLayout;
};