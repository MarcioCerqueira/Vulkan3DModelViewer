#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "structs/ImageViewInfo.h"

class ImageView
{

public:
	explicit ImageView(const ImageViewInfo& imageViewInfo);
	~ImageView();
	const vk::ImageView getVulkanImageView() const;

private:
	const vk::ImageViewCreateInfo buildImageViewCreateInfo(const ImageViewInfo& imageViewInfo) const;
	const vk::ImageSubresourceRange createImageSubresourceRange(const ImageViewInfo& imageViewInfo) const;

	const vk::Device vulkanLogicalDevice;
	vk::ImageView vulkanImageView;
};