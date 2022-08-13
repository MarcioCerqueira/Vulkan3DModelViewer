#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class ImageView
{

public:
	ImageView(const vk::Device& vulkanLogicalDevice, const vk::Image& image, const vk::Format& format);
	~ImageView();
	const vk::ImageView getVulkanImageView() const;

private:
	const vk::ImageViewCreateInfo buildImageViewCreateInfo(const vk::Image& image, const vk::Format& format) const;
	const vk::ImageSubresourceRange createImageSubresourceRange() const;

	const vk::Device vulkanLogicalDevice;
	vk::ImageView vulkanImageView;
};