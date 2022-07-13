#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>

class Queue
{
public:
	Queue(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t>& queueFamilyIndex);
	virtual ~Queue() {};
protected:
	vk::Queue vulkanQueue;
};