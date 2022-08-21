#pragma once

#include <vulkan/vulkan.hpp>

struct ImageMemoryBarrierInfo
{
	vk::AccessFlags srcAccessMask;
	vk::AccessFlags dstAccessMask;
	vk::ImageLayout oldLayout;
	vk::ImageLayout newLayout;
	uint32_t baseMipLevel;
	uint32_t mipLevels;
};