#include "ImageView.h"

ImageView::ImageView(const vk::Device& vulkanLogicalDevice, const vk::Image& image, const vk::Format& format) : vulkanLogicalDevice(vulkanLogicalDevice)
{
    vk::ImageViewCreateInfo imageViewCreateInfo{ buildImageViewCreateInfo(image, format) };
    vulkanImageView = vulkanLogicalDevice.createImageView(imageViewCreateInfo);
}

ImageView::~ImageView()
{
    vulkanLogicalDevice.destroyImageView(vulkanImageView);
}

const vk::ImageViewCreateInfo ImageView::buildImageViewCreateInfo(const vk::Image& image, const vk::Format& format) const
{
    return vk::ImageViewCreateInfo{
        .image = image,
        .viewType = vk::ImageViewType::e2D,
        .format = format,
        .components = vk::ComponentSwizzle::eIdentity,
        .subresourceRange = createImageSubresourceRange()
    };
}

const vk::ImageSubresourceRange ImageView::createImageSubresourceRange() const
{
    return vk::ImageSubresourceRange{
        .aspectMask = vk::ImageAspectFlagBits::eColor,
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
