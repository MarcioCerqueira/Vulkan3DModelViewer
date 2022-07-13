#include "Queue.h"

Queue::Queue(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t>& queueFamilyIndex)
{
	vulkanQueue = vulkanLogicalDevice.getQueue(queueFamilyIndex.value(), 0);
}