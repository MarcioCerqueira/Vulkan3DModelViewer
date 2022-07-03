#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <optional>
#include <iostream>
#include <set>

#include "LogicalDeviceCreateInfo.h"
#include "SwapChain.h"
#include "RenderPass.h"
#include "Framebuffer.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "GraphicsPipeline.h"
#include "Shader.h"

class LogicalDevice
{
public:
	explicit LogicalDevice(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo);
	~LogicalDevice();

	void createGraphicsPipeline(const std::vector<std::shared_ptr<Shader>>& shaders);

	vk::Device getVulkanLogicalDevice() const noexcept;

private:
	std::set<uint32_t> createUniqueQueueFamilies(const QueueFamilyIndices& queueFamilyIndices) const;
	std::vector<vk::DeviceQueueCreateInfo> buildDeviceQueueCreateInfos(const std::set<uint32_t>& uniqueQueueFamilies) const;
	vk::DeviceQueueCreateInfo buildDeviceQueueCreateInfo(uint32_t queueFamilyIndex) const;
	vk::DeviceCreateInfo buildVulkanLogicalDeviceCreateInfo(const std::vector<vk::DeviceQueueCreateInfo>& deviceQueueCreateInfos, const LogicalDeviceCreateInfo& logicalDeviceCreateInfo) const;
	void createSwapChain(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo);
	void createRenderPass();
	void createFramebuffers();
	void createCommandPool(const std::optional<uint32_t> graphicsFamilyIndex);
	void createCommandBuffer();

	vk::Device vulkanLogicalDevice; 
	std::unique_ptr<SwapChain> swapChain;
	std::unique_ptr<RenderPass> renderPass;
	std::unique_ptr<GraphicsPipeline> graphicsPipeline;
	std::vector<std::unique_ptr<Framebuffer>> framebuffers;
	std::unique_ptr<CommandPool> commandPool;
	std::unique_ptr<CommandBuffer> commandBuffer;
	vk::PhysicalDeviceFeatures physicalDeviceFeatures;
};