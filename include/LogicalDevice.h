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
#include "SynchronizationObjects.h"
#include "GraphicsQueue.h"
#include "PresentQueue.h"
 
class LogicalDevice
{
public:
	explicit LogicalDevice(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo);
	~LogicalDevice();

	void createGraphicsPipeline(const std::vector<std::shared_ptr<Shader>>& shaders);
	void drawFrame();
	void waitIdle();

	const vk::Device getVulkanLogicalDevice() const;

private:
	const std::set<uint32_t> createUniqueQueueFamilies(const QueueFamilyIndices& queueFamilyIndices) const;
	const std::vector<vk::DeviceQueueCreateInfo> buildDeviceQueueCreateInfos(const std::set<uint32_t>& uniqueQueueFamilies) const;
	const vk::DeviceQueueCreateInfo buildDeviceQueueCreateInfo(uint32_t queueFamilyIndex) const;
	const vk::DeviceCreateInfo buildVulkanLogicalDeviceCreateInfo(const std::vector<vk::DeviceQueueCreateInfo>& deviceQueueCreateInfos, const LogicalDeviceCreateInfo& logicalDeviceCreateInfo) const;
	void createSwapChain(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo);
	void createRenderPass();
	void createFramebuffers();
	void createCommandPool(const std::optional<uint32_t> graphicsFamilyIndex);
	void createCommandBuffers();
	void createSynchronizationObjects();
	void createQueues(const QueueFamilyIndices& queueFamilyIndices);

	const int MAX_FRAMES_IN_FLIGHT = 2;
	unsigned int currentFrame = 0;
	vk::Device vulkanLogicalDevice; 
	std::unique_ptr<SwapChain> swapChain;
	std::unique_ptr<RenderPass> renderPass;
	std::unique_ptr<GraphicsPipeline> graphicsPipeline;
	std::vector<std::unique_ptr<Framebuffer>> framebuffers;
	std::unique_ptr<CommandPool> commandPool;
	std::unique_ptr<CommandBuffer> commandBuffers;
	std::unique_ptr<GraphicsQueue> graphicsQueue;
	std::unique_ptr<PresentQueue> presentQueue;
	std::vector<std::shared_ptr<SynchronizationObjects>> synchronizationObjects;
	vk::PhysicalDeviceFeatures physicalDeviceFeatures;
};