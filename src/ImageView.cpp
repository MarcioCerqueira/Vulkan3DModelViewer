#include "ImageView.h"

ImageView::ImageView(const ImageViewInfo& imageViewInfo) : vulkanLogicalDevice(imageViewInfo.vulkanLogicalDevice)
{
    vk::ImageViewCreateInfo imageViewCreateInfo{ buildImageViewCreateInfo(imageViewInfo.image, imageViewInfo.format, imageViewInfo.aspectMask) };
    vulkanImageView = vulkanLogicalDevice.createImageView(imageViewCreateInfo);
}

ImageView::~ImageView()
{
    vulkanLogicalDevice.destroyImageView(vulkanImageView);
}

const vk::ImageViewCreateInfo ImageView::buildImageViewCreateInfo(const vk::Image& image, const vk::Format& format, const vk::ImageAspectFlags& aspectMask) const
{
    return vk::ImageViewCreateInfo{
        .image = image,
        .viewType = vk::ImageViewType::e2D,
        .format = format,
        .components = vk::ComponentSwizzle::eIdentity,
        .subresourceRange = createImageSubresourceRange(aspectMask)
    };
}

const vk::ImageSubresourceRange ImageView::createImageSubresourceRange(const vk::ImageAspectFlags& aspectMask) const
{
    return vk::ImageSubresourceRange{
        .aspectMask = aspectMask,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1
    };
}

const vk::ImageView ImageView::getVulkanImageView() const
{
    return vulkanImageView;
}
