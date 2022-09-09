#include "CommandPool.h"

CommandPool::CommandPool(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t> graphicsFamilyIndex) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	const vk::CommandPoolCreateInfo commandPoolCreateInfo{ buildCommandPoolCreateInfo(graphicsFamilyIndex) };
	vulkanCommandPool = vulkanLogicalDevice.createCommandPool(commandPoolCreateInfo);
}

CommandPool::~CommandPool()
{
	vulkanLogicalDevice.destroyCommandPool(vulkanCommandPool);
}

vk::CommandPoolCreateInfo CommandPool::buildCommandPoolCreateInfo(const std::optional<uint32_t> graphicsFamilyIndex) const
{
	return vk::CommandPoolCreateInfo{
		.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		.queueFamilyIndex = graphicsFamilyIndex.value()
	};
}

const vk::CommandPool& CommandPool::getVulkanCommandPool() const
{
	return vulkanCommandPool;
}