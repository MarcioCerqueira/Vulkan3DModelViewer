#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>
#include <iostream>
#include <set>
#include <chrono>

#include "structs/LogicalDeviceCreateInfo.h"
#include "structs/ModelViewProjectionTransformation.h"
#include "SwapChain.h"
#include "RenderPass.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "GraphicsPipeline.h"
#include "Shader.h"
#include "SynchronizationObjects.h"
#include "GraphicsQueue.h"
#include "PresentQueue.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "DescriptorSetLayout.h"
 
class LogicalDevice
{
public:
	explicit LogicalDevice(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo);
	~LogicalDevice();

	void createGraphicsPipeline(const std::vector<std::shared_ptr<Shader>>& shaders);
	void drawFrame(std::function<WindowSize()> getFramebufferSize, std::function<void()> waitEvents);
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
	void createVertexBuffer(const std::vector<Vertex>& vertices, const vk::PhysicalDevice& vulkanPhysicalDevice);
	template<typename T>
	const ContentBufferCreateInfo<T> buildContentBufferCreateInfo(const std::vector<T>& content, const vk::PhysicalDevice& vulkanPhysicalDevice) const;
	void createIndexBuffer(const std::vector<uint16_t>& indices, const vk::PhysicalDevice& vulkanPhysicalDevice);
	void createUniformBuffers(const vk::PhysicalDevice& vulkanPhysicalDevice);
	void createDescriptorSetLayout();
	void waitForFences(const uint32_t fenceCount);
	const uint32_t acquireNextImageFromSwapChain(std::function<WindowSize()> getFramebufferSize, std::function<void()> waitEvents);
	void recreateSwapChainIfResultIsOutOfDateOrSuboptimalKHR(vk::Result& result, std::function<WindowSize()> getFramebufferSize, std::function<void()> waitEvents);
	void resetFences(const uint32_t fenceCount);
	const CommandBufferRecordInfo createCommandBufferRecordInfo(const uint32_t imageIndex) const;
	void updateUniformBuffer();
	void presentResult(std::function<WindowSize()> getFramebufferSize, std::function<void()> waitEvents, const uint32_t imageIndex);

	const int MAX_FRAMES_IN_FLIGHT = 2;
	unsigned int currentFrame = 0;
	vk::Device vulkanLogicalDevice; 
	std::unique_ptr<SwapChain> swapChain;
	std::unique_ptr<RenderPass> renderPass;
	std::unique_ptr<DescriptorSetLayout> descriptorSetLayout;
	std::unique_ptr<GraphicsPipeline> graphicsPipeline;
	std::unique_ptr<CommandPool> commandPool;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::vector<std::unique_ptr<UniformBuffer>> uniformBuffers;
	std::unique_ptr<CommandBuffer> commandBuffers;
	std::shared_ptr<GraphicsQueue> graphicsQueue;
	std::unique_ptr<PresentQueue> presentQueue;
	std::vector<std::shared_ptr<SynchronizationObjects>> synchronizationObjects;
	vk::PhysicalDeviceFeatures physicalDeviceFeatures;
};