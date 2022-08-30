#include "Image.h"

const std::unordered_map<vk::ImageLayout, vk::PipelineStageFlags> pipelineStage = {
	{vk::ImageLayout::eUndefined, vk::PipelineStageFlagBits::eTopOfPipe},
	{vk::ImageLayout::eTransferDstOptimal, vk::PipelineStageFlagBits::eTransfer},
	{vk::ImageLayout::eTransferSrcOptimal, vk::PipelineStageFlagBits::eTransfer},
	{vk::ImageLayout::eShaderReadOnlyOptimal, vk::PipelineStageFlagBits::eFragmentShader}
};

Image::Image(const ImageInfo& imageInfo) : vulkanLogicalDevice(imageInfo.vulkanLogicalDevice), width(imageInfo.width), height(imageInfo.height), mipLevels(imageInfo.mipLevels), format(imageInfo.format), sampler(vulkanLogicalDevice, imageInfo.physicalDeviceProperties.getVulkanPhysicalDevice().getProperties(), mipLevels)
{
	imageLayout = vk::ImageLayout::eUndefined;
	const vk::ImageCreateInfo imageCreateInfo{ buildImageCreateInfo(imageInfo.usageFlags, imageInfo.sampleCount) };
	vulkanImage = vulkanLogicalDevice.createImage(imageCreateInfo);
	const vk::MemoryRequirements memoryRequirements{ vulkanLogicalDevice.getImageMemoryRequirements(vulkanImage) };
	const uint32_t memoryTypeIndex{ imageInfo.physicalDeviceProperties.findMemoryType(memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal) };
	const vk::MemoryAllocateInfo memoryAllocateInfo{ buildMemoryAllocateInfo(memoryRequirements, memoryTypeIndex) };
	vulkanImageMemory = vulkanLogicalDevice.allocateMemory(memoryAllocateInfo);
	const vk::DeviceSize memoryOffset{ 0 };
	vulkanLogicalDevice.bindImageMemory(vulkanImage, vulkanImageMemory, memoryOffset);
}

Image::~Image()
{
	imageView.reset();
	vulkanLogicalDevice.destroyImage(vulkanImage);
	vulkanLogicalDevice.freeMemory(vulkanImageMemory);
}

const vk::ImageCreateInfo Image::buildImageCreateInfo(const vk::ImageUsageFlags& imageUsageFlags, const vk::SampleCountFlagBits& sampleCountFlagBits) const
{
	return vk::ImageCreateInfo{
		.imageType = vk::ImageType::e2D,
		.format = format,
		.extent = vk::Extent3D(static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1),
		.mipLevels = mipLevels,
		.arrayLayers = 1,
		.samples = sampleCountFlagBits,
		.tiling = vk::ImageTiling::eOptimal,
		.usage = imageUsageFlags,
		.sharingMode = vk::SharingMode::eExclusive,
		.initialLayout = imageLayout
	};
}

const vk::MemoryAllocateInfo Image::buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const
{
	return vk::MemoryAllocateInfo{
		.allocationSize = memoryRequirements.size,
		.memoryTypeIndex = memoryTypeIndex
	};
}

void Image::transitionLayout(const ImageMemoryBarrierInfo& imageMemoryBarrierInfo, std::shared_ptr<CommandBuffer>& commandBuffers)
{
	const vk::ImageMemoryBarrier imageMemoryBarrier{ buildImageMemoryBarrier(imageMemoryBarrierInfo, 0, mipLevels) };
	const CommandBufferPipelineBarrierInfo commandBufferPipelineBarrierInfo{ buildCommandBufferPipelineBarrierInfo(imageMemoryBarrierInfo, imageMemoryBarrier) };
	commandBuffers->beginOneTimeSubmit(commandBufferPipelineBarrierInfo.frameIndex);
	commandBuffers->pipelineBarrier(commandBufferPipelineBarrierInfo);
	commandBuffers->endOneTimeSubmit(commandBufferPipelineBarrierInfo.frameIndex);
	imageLayout = imageMemoryBarrierInfo.newLayout;
}

void Image::createImageView(const vk::ImageAspectFlags& aspectMask)
{
	const ImageViewInfo imageViewInfo{
		.vulkanLogicalDevice = vulkanLogicalDevice,
		.image = vulkanImage,
		.format = format,
		.aspectMask = aspectMask,
		.mipLevels = mipLevels
	};
	imageView = std::make_unique<ImageView>(imageViewInfo);
}

vk::ImageMemoryBarrier Image::buildImageMemoryBarrier(const ImageMemoryBarrierInfo& imageMemoryBarrierInfo, const uint32_t baseMipLevel, const uint32_t mipLevels) const
{
	return vk::ImageMemoryBarrier{
		.srcAccessMask = imageMemoryBarrierInfo.srcAccessMask,
		.dstAccessMask = imageMemoryBarrierInfo.dstAccessMask,
		.oldLayout = imageMemoryBarrierInfo.oldLayout,
		.newLayout = imageMemoryBarrierInfo.newLayout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = vulkanImage,
		.subresourceRange = buildImageSubresourceRange(baseMipLevel, mipLevels)
	};
}

const vk::ImageSubresourceRange Image::buildImageSubresourceRange(const uint32_t baseMipLevel, const uint32_t mipLevels) const
{
	return vk::ImageSubresourceRange{
		.aspectMask = vk::ImageAspectFlagBits::eColor,
		.baseMipLevel = baseMipLevel,
		.levelCount = mipLevels,
		.baseArrayLayer = 0,
		.layerCount = 1
	};
}

const CommandBufferPipelineBarrierInfo Image::buildCommandBufferPipelineBarrierInfo(const ImageMemoryBarrierInfo& imageMemoryBarrierInfo, const vk::ImageMemoryBarrier& imageMemoryBarrier) const
{
	return CommandBufferPipelineBarrierInfo{
		.frameIndex = 0,
		.srcStage = pipelineStage.at(imageMemoryBarrierInfo.oldLayout),
		.dstStage = pipelineStage.at(imageMemoryBarrierInfo.newLayout),
		.imageMemoryBarrier = imageMemoryBarrier
	};
}

void Image::generateMipmaps(std::shared_ptr<CommandBuffer>& commandBuffers, const PhysicalDeviceProperties& physicalDeviceProperties)
{
	commandBuffers->beginOneTimeSubmit(0);
	checkLinearBlittingSupport(physicalDeviceProperties);
	int32_t mipWidth{ width };
	int32_t mipHeight{ height };
	for (uint32_t level = 1; level < mipLevels; ++level)
	{
		const ImageMemoryBarrierInfo imageMemoryBarrierInfo{ prepareMipLevelForBlit(commandBuffers, level - 1) };
		const vk::ImageBlit imageBlit{ buildImageBlit(level, mipWidth, mipHeight) };
		const CommandBufferBlitImageInfo commandBufferBlitImageInfo{ buildCommandBufferBlitImageInfo(imageMemoryBarrierInfo.newLayout, imageMemoryBarrierInfo.oldLayout, imageBlit) };
		commandBuffers->blitImage(commandBufferBlitImageInfo);
		transferMipLevelToShaderLayout(commandBuffers, level - 1);
		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}
	transferLastMipLevelToShaderLayout(commandBuffers);
	commandBuffers->endOneTimeSubmit(0);
	imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
}

void Image::checkLinearBlittingSupport(const PhysicalDeviceProperties& physicalDeviceProperties) const
{
	const vk::FormatProperties formatProperties{ physicalDeviceProperties.getVulkanPhysicalDevice().getFormatProperties(format) };
	if (!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear))
	{
		throw std::runtime_error("Texture image format does not support linear blitting!");
	}
}

const vk::ImageBlit Image::buildImageBlit(const uint32_t level, const int32_t mipWidth, const int32_t mipHeight) const
{
	std::array<vk::Offset3D, 2> srcOffsets, dstOffsets;
	srcOffsets[0] = vk::Offset3D{ 0, 0, 0 };
	srcOffsets[1] = vk::Offset3D{ mipWidth, mipHeight, 1 };
	dstOffsets[0] = vk::Offset3D{ 0, 0, 0 };
	dstOffsets[1] = vk::Offset3D{ mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
	return vk::ImageBlit{
		.srcSubresource = buildImageSubresourceLayers(level - 1),
		.srcOffsets = srcOffsets,
		.dstSubresource = buildImageSubresourceLayers(level),
		.dstOffsets = dstOffsets
	};
}

const vk::ImageSubresourceLayers Image::buildImageSubresourceLayers(const uint32_t mipLevel) const
{
	return vk::ImageSubresourceLayers{
		.aspectMask = vk::ImageAspectFlagBits::eColor,
		.mipLevel = mipLevel,
		.baseArrayLayer = 0,
		.layerCount = 1
	};
}

const CommandBufferBlitImageInfo Image::buildCommandBufferBlitImageInfo(const vk::ImageLayout& srcLayout, const vk::ImageLayout& dstLayout, const vk::ImageBlit& imageBlit) const
{
	return CommandBufferBlitImageInfo{
		.frameIndex = 0,
		.image = vulkanImage,
		.srcLayout = srcLayout,
		.dstLayout = dstLayout,
		.blit = imageBlit
	};
}

const ImageMemoryBarrierInfo Image::prepareMipLevelForBlit(std::shared_ptr<CommandBuffer>& commandBuffers, const int level)
{
	const ImageMemoryBarrierInfo imageMemoryBarrierInfo{
		.srcAccessMask = vk::AccessFlagBits::eTransferWrite,
		.dstAccessMask = vk::AccessFlagBits::eTransferRead,
		.oldLayout = vk::ImageLayout::eTransferDstOptimal,
		.newLayout = vk::ImageLayout::eTransferSrcOptimal
	};
	const vk::ImageMemoryBarrier imageMemoryBarrier{ buildImageMemoryBarrier(imageMemoryBarrierInfo, level, 1) };
	const CommandBufferPipelineBarrierInfo commandBufferPipelineBarrierInfo{ buildCommandBufferPipelineBarrierInfo(imageMemoryBarrierInfo, imageMemoryBarrier) };
	commandBuffers->pipelineBarrier(commandBufferPipelineBarrierInfo);
	return imageMemoryBarrierInfo;
}

void Image::transferMipLevelToShaderLayout(std::shared_ptr<CommandBuffer>& commandBuffers, const int level)
{
	const ImageMemoryBarrierInfo imageMemoryBarrierInfo{
		.srcAccessMask = vk::AccessFlagBits::eTransferRead,
		.dstAccessMask = vk::AccessFlagBits::eShaderRead,
		.oldLayout = vk::ImageLayout::eTransferSrcOptimal,
		.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal
	};
	const vk::ImageMemoryBarrier imageMemoryBarrier{ buildImageMemoryBarrier(imageMemoryBarrierInfo, level, 1) };
	const CommandBufferPipelineBarrierInfo commandBufferPipelineBarrierInfo{ buildCommandBufferPipelineBarrierInfo(imageMemoryBarrierInfo, imageMemoryBarrier) };
	commandBuffers->pipelineBarrier(commandBufferPipelineBarrierInfo);
}

void Image::transferLastMipLevelToShaderLayout(std::shared_ptr<CommandBuffer>& commandBuffers)
{
	const ImageMemoryBarrierInfo imageMemoryBarrierInfo{
		.srcAccessMask = vk::AccessFlagBits::eTransferWrite,
		.dstAccessMask = vk::AccessFlagBits::eShaderRead,
		.oldLayout = vk::ImageLayout::eTransferDstOptimal,
		.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal
	};
	const vk::ImageMemoryBarrier imageMemoryBarrier{ buildImageMemoryBarrier(imageMemoryBarrierInfo, mipLevels - 1, 1) };
	const CommandBufferPipelineBarrierInfo commandBufferPipelineBarrierInfo{ buildCommandBufferPipelineBarrierInfo(imageMemoryBarrierInfo, imageMemoryBarrier) };
	commandBuffers->pipelineBarrier(commandBufferPipelineBarrierInfo);
}

const vk::Image Image::getVulkanImage() const
{
	return vulkanImage;
}

const vk::ImageView Image::getVulkanImageView() const
{
	return imageView->getVulkanImageView();
}

const vk::Sampler Image::getVulkanSampler() const
{
	return sampler.getVulkanSampler();
}

const vk::ImageLayout Image::getImageLayout() const
{
	return imageLayout;
}

const vk::BufferImageCopy Image::buildBufferImageCopy() const
{
	return vk::BufferImageCopy{
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource = buildImageSubresourceLayers(0),
		.imageOffset = {0, 0, 0},
		.imageExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1}
	};
}