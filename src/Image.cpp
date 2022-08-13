#include "Image.h"

Image::Image(const ImageInfo& imageInfo) : vulkanLogicalDevice(imageInfo.vulkanLogicalDevice), width(imageInfo.width), height(imageInfo.height), format(vk::Format::eR8G8B8A8Srgb), sampler(vulkanLogicalDevice, imageInfo.vulkanPhysicalDevice.getProperties())
{
	const vk::ImageCreateInfo imageCreateInfo{ buildImageCreateInfo() };
	vulkanImage = vulkanLogicalDevice.createImage(imageCreateInfo);
	const vk::MemoryRequirements memoryRequirements{ vulkanLogicalDevice.getImageMemoryRequirements(vulkanImage) };
	const uint32_t memoryTypeIndex{ MemoryProperties::findMemoryType(imageInfo.vulkanPhysicalDevice, memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal) };
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

const vk::ImageCreateInfo Image::buildImageCreateInfo() const
{
	return vk::ImageCreateInfo{
		.imageType = vk::ImageType::e2D,
		.format = format,
		.extent = vk::Extent3D(static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1),
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = vk::SampleCountFlagBits::e1,
		.tiling = vk::ImageTiling::eOptimal,
		.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
		.sharingMode = vk::SharingMode::eExclusive,
		.initialLayout = vk::ImageLayout::eUndefined
	};
}

const vk::MemoryAllocateInfo Image::buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const
{
	return vk::MemoryAllocateInfo{
		.allocationSize = memoryRequirements.size,
		.memoryTypeIndex = memoryTypeIndex
	};
}

void Image::transitionLayout(const vk::ImageLayout& oldLayout, const vk::ImageLayout& newLayout, std::shared_ptr<CommandBuffer>& commandBuffers)
{
	const vk::ImageMemoryBarrier imageMemoryBarrier{ buildImageMemoryBarrier(oldLayout, newLayout) };
	vk::PipelineStageFlags srcPipelineStage, dstPipelineStage;
	std::tie(srcPipelineStage, dstPipelineStage) = determinePipelineStages(oldLayout, newLayout);
	const CommandBufferPipelineBarrierInfo commandBufferPipelineBarrierInfo{ buildCommandBufferPipelineBarrierInfo(srcPipelineStage, dstPipelineStage, imageMemoryBarrier) };
	commandBuffers->pipelineBarrier(commandBufferPipelineBarrierInfo);
}

void Image::createImageView()
{
	imageView = std::make_unique<ImageView>(vulkanLogicalDevice, vulkanImage, format);
}

const vk::ImageMemoryBarrier Image::buildImageMemoryBarrier(const vk::ImageLayout& oldLayout, const vk::ImageLayout& newLayout) const
{
	return vk::ImageMemoryBarrier{
		.srcAccessMask = (oldLayout == vk::ImageLayout::eUndefined) ? vk::AccessFlagBits::eNoneKHR : vk::AccessFlagBits::eTransferWrite,
		.dstAccessMask = (newLayout == vk::ImageLayout::eTransferDstOptimal) ? vk::AccessFlagBits::eTransferWrite : vk::AccessFlagBits::eShaderRead,
		.oldLayout = oldLayout,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = vulkanImage,
		.subresourceRange = buildImageSubresourceRange()
	};
}

const vk::ImageSubresourceRange Image::buildImageSubresourceRange() const
{
	return vk::ImageSubresourceRange{
		.aspectMask = vk::ImageAspectFlagBits::eColor,
		.baseMipLevel = 0,
		.levelCount = 1,
		.baseArrayLayer = 0,
		.layerCount = 1
	};
}

std::tuple<vk::PipelineStageFlags, vk::PipelineStageFlags> Image::determinePipelineStages(const vk::ImageLayout& oldLayout, const vk::ImageLayout& newLayout) const
{
	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
	{
		return { vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer };
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
	{
		return { vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader };
	}
	else
	{
		throw std::invalid_argument("Unsupported layout transition!");
	}
}

const CommandBufferPipelineBarrierInfo Image::buildCommandBufferPipelineBarrierInfo(const vk::PipelineStageFlags& srcStage, const vk::PipelineStageFlags& dstStage, const vk::ImageMemoryBarrier& imageMemoryBarrier) const
{
	return CommandBufferPipelineBarrierInfo{
		.frameIndex = 0,
		.srcStage = srcStage,
		.dstStage = dstStage,
		.imageMemoryBarrier = imageMemoryBarrier
	};
}

const vk::Image Image::getVulkanImage() const
{
	return vulkanImage;
}

const vk::BufferImageCopy Image::buildBufferImageCopy() const
{
	return vk::BufferImageCopy{
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource = buildImageSubresourceLayers(),
		.imageOffset = {0, 0, 0},
		.imageExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1}
	};
}

const vk::ImageSubresourceLayers Image::buildImageSubresourceLayers() const
{
	return vk::ImageSubresourceLayers{
		.aspectMask = vk::ImageAspectFlagBits::eColor,
		.mipLevel = 0,
		.baseArrayLayer = 0,
		.layerCount = 1
	};
}
