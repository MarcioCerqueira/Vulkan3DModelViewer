#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <stdexcept>
#include "CommandBuffer.h"
#include "ImageView.h"
#include "Sampler.h"
#include "structs/ImageInfo.h"
#include "structs/ImageMemoryBarrierInfo.h"
#include "PhysicalDeviceProperties.h"

class Image
{
public:
	Image(const ImageInfo& imageInfo);
	~Image();

	void transitionLayout(const ImageMemoryBarrierInfo& imageMemoryBarrierInfo, std::shared_ptr<CommandBuffer>& commandBuffers);
	void createImageView(const vk::ImageAspectFlags& aspectMask);
	void generateMipmaps(std::shared_ptr<CommandBuffer>& commandBuffers, const PhysicalDeviceProperties& physicalDeviceProperties);
	const vk::Image& getVulkanImage() const;
	const vk::ImageView& getVulkanImageView() const;
	const vk::Sampler& getVulkanSampler() const;
	const vk::ImageLayout& getImageLayout() const;
	vk::BufferImageCopy buildBufferImageCopy() const;

private:
	vk::ImageCreateInfo buildImageCreateInfo(const vk::ImageUsageFlags& imageUsageFlags, const vk::SampleCountFlagBits& sampleCountFlagBits) const;
	vk::MemoryAllocateInfo buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const;
	vk::ImageMemoryBarrier buildImageMemoryBarrier(const ImageMemoryBarrierInfo& imageMemoryBarrierInfo, const uint32_t baseMipLevel, const uint32_t mipLevels) const;
	vk::ImageSubresourceRange buildImageSubresourceRange(const uint32_t baseMipLevel, const uint32_t mipLevels) const;
	CommandBufferPipelineBarrierInfo buildCommandBufferPipelineBarrierInfo(const ImageMemoryBarrierInfo& imageMemoryBarrierInfo, const vk::ImageMemoryBarrier& imageMemoryBarrier) const;
	void checkLinearBlittingSupport(const PhysicalDeviceProperties& physicalDeviceProperties) const;
	std::vector<uint32_t> generateLevelsVector() const;
	void generateEachMipmap(std::shared_ptr<CommandBuffer>& commandBuffers, const std::vector<uint32_t>& levels);
	vk::ImageBlit buildImageBlit(const uint32_t level, const int32_t mipWidth, const int32_t mipHeight) const;
	vk::ImageSubresourceLayers buildImageSubresourceLayers(const uint32_t mipLevel) const;
	CommandBufferBlitImageInfo buildCommandBufferBlitImageInfo(const vk::ImageLayout& srcLayout, const vk::ImageLayout& dstLayout, const vk::ImageBlit& imageBlit) const;
	ImageMemoryBarrierInfo prepareMipLevelForBlit(std::shared_ptr<CommandBuffer>& commandBuffers, const int level);
	void transferMipLevelToShaderLayout(std::shared_ptr<CommandBuffer>& commandBuffers, const int level);
	void transferLastMipLevelToShaderLayout(std::shared_ptr<CommandBuffer>& commandBuffers);

	const vk::Device vulkanLogicalDevice; 
	const int width;
	const int height;
	const vk::Format format;
	const uint32_t mipLevels;
	vk::Image vulkanImage;
	std::unique_ptr<ImageView> imageView;
	vk::DeviceMemory vulkanImageMemory;
	const Sampler sampler;
	vk::ImageLayout imageLayout;
};