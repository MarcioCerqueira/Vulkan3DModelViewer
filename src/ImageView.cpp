#include "ImageView.h"

ImageView::ImageView(const ImageViewInfo& imageViewInfo) : vulkanLogicalDevice(imageViewInfo.vulkanLogicalDevice)
{
    vk::ImageViewCreateInfo imageViewCreateInfo{ buildImageViewCreateInfo(imageViewInfo) };
    vulkanImageView = vulkanLogicalDevice.createImageView(imageViewCreateInfo);
}

ImageView::~ImageView()
{
    vulkanLogicalDevice.destroyImageView(vulkanImageView);
}

vk::ImageViewCreateInfo ImageView::buildImageViewCreateInfo(const ImageViewInfo& imageViewInfo) const
{
    return vk::ImageViewCreateInfo{
        .image = imageViewInfo.image,
        .viewType = vk::ImageViewType::e2D,
        .format = imageViewInfo.format,
        .components = vk::ComponentSwizzle::eIdentity,
        .subresourceRange = createImageSubresourceRange(imageViewInfo)
    };
}

vk::ImageSubresourceRange ImageView::createImageSubresourceRange(const ImageViewInfo& imageViewInfo) const
{
    return vk::ImageSubresourceRange{
        .aspectMask = imageViewInfo.aspectMask,
        .baseMipLevel = 0,
        .levelCount = imageViewInfo.mipLevels,
        .baseArrayLayer = 0,
        .layerCount = 1
    };
}

const vk::ImageView& ImageView::getVulkanImageView() const
{
    return vulkanImageView;
}
