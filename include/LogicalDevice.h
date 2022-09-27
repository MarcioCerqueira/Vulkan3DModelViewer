#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <optional>
#include <iostream>
#include <set>
#include <chrono>

#include "structs/LogicalDeviceCreateInfo.h"
#include "structs/UniformBufferObject.h"
#include "SwapChain.h"
#include "RenderPass.h"
#include "CommandBuffer.h"
#include "GraphicsPipeline.h"
#include "Shader.h"
#include "SynchronizationObjects.h"
#include "PresentQueue.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "UniformBuffer.h"
#include "DescriptorSet.h"
#include "Image.h"
#include "CameraHandler.h"

class LogicalDevice
{
public:
	explicit LogicalDevice(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo);
	~LogicalDevice();

	void createGraphicsPipeline(const std::vector<std::shared_ptr<Shader>>& shaders, const PhysicalDeviceProperties& physicalDeviceProperties);
	void drawFrame(WindowHandler& windowHandler, CameraHandler& cameraHandler, bool framebufferResized);
	void waitIdle();

	const vk::Device& getVulkanLogicalDevice() const;

private:
	std::set<uint32_t> createUniqueQueueFamilies(const QueueFamilyIndices& queueFamilyIndices) const;
	std::vector<vk::DeviceQueueCreateInfo> buildDeviceQueueCreateInfos(const std::set<uint32_t>& uniqueQueueFamilies) const;
	vk::DeviceQueueCreateInfo buildDeviceQueueCreateInfo(uint32_t queueFamilyIndex) const;
	vk::DeviceCreateInfo buildVulkanLogicalDeviceCreateInfo(const std::vector<vk::DeviceQueueCreateInfo>& deviceQueueCreateInfos, const LogicalDeviceCreateInfo& logicalDeviceCreateInfo) const;
	vk::Format getDepthImageFormat(const PhysicalDeviceProperties& physicalDeviceProperties) const;
	void createSwapChain(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo, const vk::Format& depthImageFormat);
	void createRenderPass(const vk::Format& depthImageFormat, const PhysicalDeviceProperties& physicalDeviceProperties);
	void createPresentQueue(const QueueFamilyIndices& queueFamilyIndices);
	void createCommandBuffers(const QueueFamilyIndices& queueFamilyIndices);
	void createSynchronizationObjects();
	void createVertexBuffer(const std::vector<Vertex>& vertices, const PhysicalDeviceProperties& physicalDeviceProperties);
	template<typename T>
	ContentBufferCreateInfo<T> buildContentBufferCreateInfo(const std::vector<T>& content, const PhysicalDeviceProperties& physicalDeviceProperties) const;
	void createIndexBuffer(const std::vector<uint32_t>& indices, const PhysicalDeviceProperties& physicalDeviceProperties);
	void createUniformBuffers(const PhysicalDeviceProperties& physicalDeviceProperties);
	void createTextureBuffer(const TextureImage& textureImage, const PhysicalDeviceProperties& physicalDeviceProperties);
	void createTextureImage(const TextureImage& textureImage, const PhysicalDeviceProperties& physicalDeviceProperties);
	ImageInfo createImageInfo(const TextureImage& textureImage, const PhysicalDeviceProperties& physicalDeviceProperties) const;
	void createFramebuffers();
	void createDescriptorSet();
	void waitForFences(const uint32_t fenceCount);
	uint32_t acquireNextImageFromSwapChain(WindowHandler& windowHandler);
	void resetFences(const uint32_t fenceCount);
	CommandBufferRecordInfo createCommandBufferRecordInfo(const uint32_t imageIndex) const;
	void updateUniformBuffers(WindowHandler& windowHandler, CameraHandler& cameraHandler);
	void presentResult(WindowHandler& windowHandler, const uint32_t imageIndex, bool framebufferResized);

	const int MAX_FRAMES_IN_FLIGHT = 2;
	unsigned int currentFrame = 0;
	vk::Device vulkanLogicalDevice; 
	std::unique_ptr<SwapChain> swapChain;
	std::unique_ptr<RenderPass> renderPass;
	std::unique_ptr<GraphicsPipeline> graphicsPipeline;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::vector<std::shared_ptr<UniformBuffer>> uniformBuffers;
	std::unique_ptr<StagingBuffer> textureBuffer;
	std::shared_ptr<Image> vulkanTextureImage;
	std::unique_ptr<DescriptorSet> descriptorSet;
	std::shared_ptr<CommandBuffer> commandBuffers;
	std::unique_ptr<PresentQueue> presentQueue;
	std::vector<std::shared_ptr<SynchronizationObjects>> synchronizationObjects;
	vk::PhysicalDeviceFeatures physicalDeviceFeatures;
};