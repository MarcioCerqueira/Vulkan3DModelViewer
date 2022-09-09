#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>

class CommandPool
{
public:
	CommandPool(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t> graphicsFamilyIndex);
	~CommandPool();

	const vk::CommandPool& getVulkanCommandPool() const;

private:
	vk::CommandPoolCreateInfo buildCommandPoolCreateInfo(const std::optional<uint32_t> graphicsFamilyIndex) const;

	vk::CommandPool vulkanCommandPool;
	const vk::Device vulkanLogicalDevice;
};