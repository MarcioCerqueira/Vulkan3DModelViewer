#include "LogicalDevice.h"

LogicalDevice::LogicalDevice(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo)
{
	const std::set<uint32_t> uniqueQueueFamilies = createUniqueQueueFamilies(logicalDeviceCreateInfo.queueFamilyIndices);
	const std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos{ buildDeviceQueueCreateInfos(uniqueQueueFamilies) };
	const vk::DeviceCreateInfo vulkanLogicalDeviceCreateInfo{ buildVulkanLogicalDeviceCreateInfo(deviceQueueCreateInfos, logicalDeviceCreateInfo) };
	vulkanLogicalDevice = logicalDeviceCreateInfo.vulkanPhysicalDevice.createDevice(vulkanLogicalDeviceCreateInfo);
	createSwapChain(logicalDeviceCreateInfo);
	createRenderPass();
	createFramebuffers();
	createCommandPool(logicalDeviceCreateInfo.queueFamilyIndices.getGraphicsFamilyIndex());
	createCommandBuffers();
	createSynchronizationObjects();
	createQueues(logicalDeviceCreateInfo.queueFamilyIndices);
	createVertexBuffer(logicalDeviceCreateInfo.vertices, logicalDeviceCreateInfo.vulkanPhysicalDevice);
	createIndexBuffer(logicalDeviceCreateInfo.indices, logicalDeviceCreateInfo.vulkanPhysicalDevice);
	createUniformBuffers(logicalDeviceCreateInfo.vulkanPhysicalDevice);
	createDescriptorPool();
	createDescriptorSetLayout();
	createDescriptorSet();
}

LogicalDevice::~LogicalDevice()
{
	for (auto& synchronizationObject : synchronizationObjects)
	{
		synchronizationObject.reset();
	}
	swapChain.reset();
	commandPool.reset();
	for (auto& uniformBuffer : uniformBuffers)
	{
		uniformBuffer.reset();
	}
	descriptorPool.reset();
	descriptorSetLayout.reset();
	graphicsPipeline.reset();
	renderPass.reset();
	vertexBuffer.reset();
	indexBuffer.reset();
	vulkanLogicalDevice.destroy();
}

const std::set<uint32_t> LogicalDevice::createUniqueQueueFamilies(const QueueFamilyIndices& queueFamilyIndices) const
{
	std::set<uint32_t> uniqueQueueFamilies = {
		queueFamilyIndices.getGraphicsFamilyIndex().value(),
		queueFamilyIndices.getPresentFamilyIndex().value()
	};
	return uniqueQueueFamilies;
}

const std::vector<vk::DeviceQueueCreateInfo> LogicalDevice::buildDeviceQueueCreateInfos(const std::set<uint32_t>& uniqueQueueFamilies) const
{
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
	{
		queueCreateInfos.push_back(buildDeviceQueueCreateInfo(queueFamilyIndex));
	}
	return queueCreateInfos;
}

const vk::DeviceQueueCreateInfo LogicalDevice::buildDeviceQueueCreateInfo(uint32_t queueFamilyIndex) const
{
	const float queuePriority{ 1.0f };
	return vk::DeviceQueueCreateInfo{
		.queueFamilyIndex = queueFamilyIndex,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority
	};
}

const vk::DeviceCreateInfo LogicalDevice::buildVulkanLogicalDeviceCreateInfo(const std::vector<vk::DeviceQueueCreateInfo>& deviceQueueCreateInfos, const LogicalDeviceCreateInfo& logicalDeviceCreateInfo) const
{
	return vk::DeviceCreateInfo{
		.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size()),
		.pQueueCreateInfos = deviceQueueCreateInfos.data(),
		.enabledLayerCount = logicalDeviceCreateInfo.enabledLayerCount,
		.ppEnabledLayerNames = logicalDeviceCreateInfo.enabledLayerNames,
		.enabledExtensionCount = static_cast<uint32_t>(logicalDeviceCreateInfo.vulkanDeviceExtensions.size()),
		.ppEnabledExtensionNames = logicalDeviceCreateInfo.vulkanDeviceExtensions.data(),
		.pEnabledFeatures = &physicalDeviceFeatures
	};
}

void LogicalDevice::createSwapChain(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo)
{
	const SwapChainCreateInfo swapChainCreateInfo{
		.vulkanPhysicalDevice = logicalDeviceCreateInfo.vulkanPhysicalDevice,
		.vulkanWindowSurface = logicalDeviceCreateInfo.vulkanWindowSurface,
		.framebufferSize = logicalDeviceCreateInfo.framebufferSize,
		.queueFamilyIndices = logicalDeviceCreateInfo.queueFamilyIndices,
		.vulkanLogicalDevice = this->vulkanLogicalDevice
	};
	swapChain = std::make_unique<SwapChain>(swapChainCreateInfo);
}

void LogicalDevice::createRenderPass()
{
	renderPass = std::make_unique<RenderPass>(vulkanLogicalDevice, swapChain->getSurfaceFormat());
}

void LogicalDevice::createFramebuffers()
{
	swapChain->buildFramebuffers(vulkanLogicalDevice, renderPass->getVulkanRenderPass());
}

void LogicalDevice::createCommandPool(const std::optional<uint32_t> graphicsFamilyIndex)
{
	commandPool = std::make_unique<CommandPool>(vulkanLogicalDevice, graphicsFamilyIndex);
}

void LogicalDevice::createCommandBuffers()
{
	commandBuffers = std::make_unique<CommandBuffer>(vulkanLogicalDevice, commandPool->getVulkanCommandPool(), MAX_FRAMES_IN_FLIGHT);
}

void LogicalDevice::createSynchronizationObjects()
{
	synchronizationObjects.resize(MAX_FRAMES_IN_FLIGHT);
	for (auto& synchronizationObject : synchronizationObjects)
	{
		synchronizationObject = std::make_shared<SynchronizationObjects>(vulkanLogicalDevice);
	}
}

void LogicalDevice::createQueues(const QueueFamilyIndices& queueFamilyIndices)
{
	graphicsQueue = std::make_shared<GraphicsQueue>(vulkanLogicalDevice, queueFamilyIndices.getGraphicsFamilyIndex());
	presentQueue = std::make_unique<PresentQueue>(vulkanLogicalDevice, queueFamilyIndices.getPresentFamilyIndex());
}

void LogicalDevice::createVertexBuffer(const std::vector<Vertex>& vertices, const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	const ContentBufferCreateInfo<Vertex> contentBufferCreateInfo{ buildContentBufferCreateInfo<Vertex>(vertices, vulkanPhysicalDevice) };
	vertexBuffer = std::make_unique<VertexBuffer>(contentBufferCreateInfo);
}

template<typename T>
const ContentBufferCreateInfo<T> LogicalDevice::buildContentBufferCreateInfo(const std::vector<T>& content, const vk::PhysicalDevice& vulkanPhysicalDevice) const
{
	return ContentBufferCreateInfo<T>{
		.vulkanLogicalDevice = vulkanLogicalDevice,
		.content = content,
		.vulkanPhysicalDevice = vulkanPhysicalDevice,
		.vulkanCommandPool = commandPool->getVulkanCommandPool(),
		.graphicsQueue = graphicsQueue
	};
}

void LogicalDevice::createIndexBuffer(const std::vector<uint16_t>& indices, const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	const ContentBufferCreateInfo<uint16_t> contentBufferCreateInfo{ buildContentBufferCreateInfo<uint16_t>(indices, vulkanPhysicalDevice) };
	indexBuffer = std::make_unique<IndexBuffer>(contentBufferCreateInfo);
}

void LogicalDevice::createUniformBuffers(const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	const vk::DeviceSize bufferSize{ sizeof(ModelViewProjectionTransformation) };
	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	for (auto& uniformBuffer : uniformBuffers)
	{
		uniformBuffer = std::make_unique<UniformBuffer>(vulkanLogicalDevice, vulkanPhysicalDevice, bufferSize);
	}
}

void LogicalDevice::createDescriptorPool()
{
	descriptorPool = std::make_unique<DescriptorPool>(vulkanLogicalDevice, MAX_FRAMES_IN_FLIGHT);
}

void LogicalDevice::createDescriptorSetLayout()
{
	descriptorSetLayout = std::make_unique<DescriptorSetLayout>(vulkanLogicalDevice);
}

void LogicalDevice::createDescriptorSet()
{
	const DescriptorSetCreateInfo descriptorSetCreateInfo{
		.vulkanLogicalDevice = this->vulkanLogicalDevice,
		.vulkanDescriptorPool = descriptorPool->getVulkanDescriptorPool(),
		.vulkanDescriptorSetLayout = descriptorSetLayout->getVulkanDescriptorSetLayout(),
		.maxFramesInFlight = MAX_FRAMES_IN_FLIGHT
	};
	descriptorSet = std::make_unique<DescriptorSet>(descriptorSetCreateInfo);
}

const vk::Device LogicalDevice::getVulkanLogicalDevice() const
{
	return vulkanLogicalDevice;
}

void LogicalDevice::createGraphicsPipeline(const std::vector<std::shared_ptr<Shader>>& shaders)
{
	GraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
	graphicsPipelineCreateInfo.vulkanLogicalDevice = vulkanLogicalDevice;
	graphicsPipelineCreateInfo.swapChainExtent = swapChain->getExtent();
	for (const auto& shader : shaders)
	{
		graphicsPipelineCreateInfo.shaderStages.push_back(shader->buildPipelineShaderStageCreateInfo());
	}
	graphicsPipelineCreateInfo.vulkanRenderPass = renderPass->getVulkanRenderPass();
	graphicsPipelineCreateInfo.vulkanDescriptorSetLayout = descriptorSetLayout->getVulkanDescriptorSetLayout();
	graphicsPipeline = std::make_unique<GraphicsPipeline>(graphicsPipelineCreateInfo);
}

void LogicalDevice::drawFrame(WindowHandler& windowHandler)
{
	const uint32_t fenceCount{ 1 };
	waitForFences(fenceCount);
	const uint32_t imageIndex{ acquireNextImageFromSwapChain(windowHandler) };
	resetFences(fenceCount);
	commandBuffers->reset(currentFrame);
	updateUniformBuffer();
	descriptorSet->write(uniformBuffers[currentFrame]->getVulkanBuffer(), currentFrame);
	commandBuffers->record(createCommandBufferRecordInfo(imageIndex));
	graphicsQueue->submit(synchronizationObjects[currentFrame], commandBuffers->getVulkanCommandBuffer(currentFrame));
	presentResult(windowHandler, imageIndex);
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void LogicalDevice::waitForFences(const uint32_t fenceCount)
{
	constexpr uint64_t timeout{ std::numeric_limits<uint64_t>::max() };
	const vk::Result result{ vulkanLogicalDevice.waitForFences(fenceCount, &synchronizationObjects[currentFrame]->inFlight, vk::Bool32{ true }, timeout) };
	ExceptionChecker::throwExceptionIfVulkanResultIsNotSuccess(result, "Failed to wait for fences!");
}

const uint32_t LogicalDevice::acquireNextImageFromSwapChain(WindowHandler& windowHandler)
{
	uint32_t imageIndex;
	vk::Result result;
	try
	{
		result = swapChain->acquireNextImage(synchronizationObjects[currentFrame]->imageAvailable, renderPass->getVulkanRenderPass(), imageIndex);
	}
	catch (vk::OutOfDateKHRError error)
	{
		result = vk::Result::eErrorOutOfDateKHR;
	}
	swapChain->recreateIfResultIsOutOfDateOrSuboptimalKHR(result, renderPass->getVulkanRenderPass(), windowHandler);
	ExceptionChecker::throwExceptionIfVulkanResultIsNotSuccess(result, "Failed to acquire next image!");
	return imageIndex;
}

void LogicalDevice::resetFences(const uint32_t fenceCount)
{
	const vk::Result result{ vulkanLogicalDevice.resetFences(fenceCount, &synchronizationObjects[currentFrame]->inFlight) };
	ExceptionChecker::throwExceptionIfVulkanResultIsNotSuccess(result, "Failed to reset fences!");
}

const CommandBufferRecordInfo LogicalDevice::createCommandBufferRecordInfo(const uint32_t imageIndex) const
{
	return CommandBufferRecordInfo{
		.renderPassBeginInfo = renderPass->createRenderPassBeginInfo(swapChain->getVulkanFramebuffer(imageIndex), swapChain->getExtent()),
		.graphicsPipeline = graphicsPipeline->getVulkanPipeline(),
		.vulkanPipelineLayout = graphicsPipeline->getVulkanPipelineLayout(),
		.vulkanVertexBuffer = vertexBuffer->getVulkanBuffer(),
		.vulkanIndexBuffer = indexBuffer->getVulkanBuffer(),
		.vulkanDescriptorSet = descriptorSet->getVulkanDescriptorSet(currentFrame),
		.frameIndex = currentFrame,
		.indexCount = indexBuffer->getIndexCount(),
		.indexType = vk::IndexType::eUint16
	};
}

void LogicalDevice::updateUniformBuffer()
{
	const vk::Extent2D swapChainExtent{ swapChain->getExtent() };
	static std::chrono::steady_clock::time_point startTime{ std::chrono::high_resolution_clock::now() };
	const std::chrono::steady_clock::time_point currentTime{ std::chrono::high_resolution_clock::now() };
	const float time{ std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count() };
	ModelViewProjectionTransformation MVP{
		.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		.projection = glm::perspective(glm::radians(45.0f), swapChainExtent.width / static_cast<float>(swapChainExtent.height), 0.1f, 10.0f)
	};
	MVP.projection[1][1] *= -1;
	uniformBuffers[currentFrame]->copyFromCPUToDeviceMemory(&MVP);
}

void LogicalDevice::presentResult(WindowHandler& windowHandler, const uint32_t imageIndex)
{
	vk::Result result;
	try
	{
		result = presentQueue->presentResult(synchronizationObjects[currentFrame]->renderFinished, swapChain->getVulkanSwapChain(), imageIndex);
	}
	catch (vk::OutOfDateKHRError error)
	{
		result = vk::Result::eErrorOutOfDateKHR;
	}
	swapChain->recreateIfResultIsOutOfDateOrSuboptimalKHR(result, renderPass->getVulkanRenderPass(), windowHandler);
	ExceptionChecker::throwExceptionIfVulkanResultIsNotSuccess(result, "Failed to present the results!");
}

void LogicalDevice::waitIdle()
{
	vulkanLogicalDevice.waitIdle();
}