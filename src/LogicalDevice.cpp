#include "LogicalDevice.h"

LogicalDevice::LogicalDevice(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo)
{
	physicalDeviceFeatures.samplerAnisotropy = vk::Bool32{ true };
	const std::set<uint32_t> uniqueQueueFamilies = createUniqueQueueFamilies(logicalDeviceCreateInfo.queueFamilyIndices);
	const std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos{ buildDeviceQueueCreateInfos(uniqueQueueFamilies) };
	const vk::DeviceCreateInfo vulkanLogicalDeviceCreateInfo{ buildVulkanLogicalDeviceCreateInfo(deviceQueueCreateInfos, logicalDeviceCreateInfo) };
	vulkanLogicalDevice = logicalDeviceCreateInfo.physicalDeviceProperties.getVulkanPhysicalDevice().createDevice(vulkanLogicalDeviceCreateInfo);
	const vk::Format depthImageFormat{ getDepthImageFormat(logicalDeviceCreateInfo.physicalDeviceProperties) };
	createSwapChain(logicalDeviceCreateInfo, depthImageFormat);
	createRenderPass(depthImageFormat);
	createPresentQueue(logicalDeviceCreateInfo.queueFamilyIndices);
	createCommandBuffers(logicalDeviceCreateInfo.queueFamilyIndices);
	createSynchronizationObjects();
	createVertexBuffer(logicalDeviceCreateInfo.model.getVertices(), logicalDeviceCreateInfo.physicalDeviceProperties);
	createIndexBuffer(logicalDeviceCreateInfo.model.getIndices(), logicalDeviceCreateInfo.physicalDeviceProperties);
	createUniformBuffers(logicalDeviceCreateInfo.physicalDeviceProperties);
	createTextureBuffer(logicalDeviceCreateInfo.model.getTextureImage(), logicalDeviceCreateInfo.physicalDeviceProperties);
	createTextureImage(logicalDeviceCreateInfo.model.getTextureImage(), logicalDeviceCreateInfo.physicalDeviceProperties);
	createFramebuffers();
	createDescriptorSet();
}

LogicalDevice::~LogicalDevice()
{
	for (auto& synchronizationObject : synchronizationObjects)
	{
		synchronizationObject.reset();
	}
	swapChain.reset();
	commandBuffers.reset();
	for (auto& uniformBuffer : uniformBuffers)
	{
		uniformBuffer.reset();
	}
	descriptorSet.reset();
	vulkanTextureImage.reset();
	graphicsPipeline.reset();
	renderPass.reset();
	vertexBuffer.reset();
	indexBuffer.reset();
	textureBuffer.reset();
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

const vk::Format LogicalDevice::getDepthImageFormat(const PhysicalDeviceProperties& physicalDeviceProperties) const
{
	const std::vector<vk::Format> depthImageCandidateFormats = { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint };
	const vk::ImageTiling depthImageTiling{ vk::ImageTiling::eOptimal };
	const vk::FormatFeatureFlags depthFormatFeatureFlags{ vk::FormatFeatureFlagBits::eDepthStencilAttachment };
	return physicalDeviceProperties.findSupportedFormat(depthImageCandidateFormats, depthImageTiling, depthFormatFeatureFlags);
}

void LogicalDevice::createSwapChain(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo, const vk::Format& depthImageFormat)
{
	const SwapChainCreateInfo swapChainCreateInfo{
		.physicalDeviceProperties = logicalDeviceCreateInfo.physicalDeviceProperties,
		.vulkanWindowSurface = logicalDeviceCreateInfo.vulkanWindowSurface,
		.framebufferSize = logicalDeviceCreateInfo.framebufferSize,
		.queueFamilyIndices = logicalDeviceCreateInfo.queueFamilyIndices,
		.vulkanLogicalDevice = this->vulkanLogicalDevice,
		.depthImageFormat = depthImageFormat
	};
	swapChain = std::make_unique<SwapChain>(swapChainCreateInfo);
}

void LogicalDevice::createRenderPass(const vk::Format& depthImageFormat)
{
	renderPass = std::make_unique<RenderPass>(vulkanLogicalDevice, swapChain->getSurfaceFormat().format, depthImageFormat);
}

void LogicalDevice::createPresentQueue(const QueueFamilyIndices& queueFamilyIndices)
{
	presentQueue = std::make_unique<PresentQueue>(vulkanLogicalDevice, queueFamilyIndices.getPresentFamilyIndex());
}

void LogicalDevice::createCommandBuffers(const QueueFamilyIndices& queueFamilyIndices)
{
	commandBuffers = std::make_unique<CommandBuffer>(vulkanLogicalDevice, queueFamilyIndices.getGraphicsFamilyIndex(), MAX_FRAMES_IN_FLIGHT);
}

void LogicalDevice::createSynchronizationObjects()
{
	synchronizationObjects.resize(MAX_FRAMES_IN_FLIGHT);
	for (auto& synchronizationObject : synchronizationObjects)
	{
		synchronizationObject = std::make_shared<SynchronizationObjects>(vulkanLogicalDevice);
	}
}

void LogicalDevice::createVertexBuffer(const std::vector<Vertex>& vertices, const PhysicalDeviceProperties& physicalDeviceProperties)
{
	const ContentBufferCreateInfo<Vertex> contentBufferCreateInfo{ buildContentBufferCreateInfo<Vertex>(vertices, physicalDeviceProperties) };
	vertexBuffer = std::make_unique<VertexBuffer>(contentBufferCreateInfo);
}

template<typename T>
const ContentBufferCreateInfo<T> LogicalDevice::buildContentBufferCreateInfo(const std::vector<T>& content, const PhysicalDeviceProperties& physicalDeviceProperties) const
{
	return ContentBufferCreateInfo<T>{
		.vulkanLogicalDevice = vulkanLogicalDevice,
		.content = content,
		.physicalDeviceProperties = physicalDeviceProperties,
		.commandBuffers = commandBuffers
	};
}

void LogicalDevice::createIndexBuffer(const std::vector<uint16_t>& indices, const PhysicalDeviceProperties& physicalDeviceProperties)
{
	const ContentBufferCreateInfo<uint16_t> contentBufferCreateInfo{ buildContentBufferCreateInfo<uint16_t>(indices, physicalDeviceProperties) };
	indexBuffer = std::make_unique<IndexBuffer>(contentBufferCreateInfo);
}

void LogicalDevice::createUniformBuffers(const PhysicalDeviceProperties& physicalDeviceProperties)
{
	const vk::DeviceSize bufferSize{ sizeof(ModelViewProjectionTransformation) };
	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	for (auto& uniformBuffer : uniformBuffers)
	{
		uniformBuffer = std::make_shared<UniformBuffer>(vulkanLogicalDevice, physicalDeviceProperties, bufferSize);
	}
}

void LogicalDevice::createTextureBuffer(const TextureImage& textureImage, const PhysicalDeviceProperties& physicalDeviceProperties)
{
	textureBuffer = std::make_unique<StagingBuffer>(vulkanLogicalDevice);
	textureBuffer->createStagingData(textureImage.getSize(), physicalDeviceProperties);
	textureBuffer->copyFromCPUToStagingMemory(textureImage.getPixels());
}

void LogicalDevice::createTextureImage(const TextureImage& textureImage, const PhysicalDeviceProperties& physicalDeviceProperties)
{
	const ImageInfo imageInfo{ createImageInfo(textureImage, physicalDeviceProperties) };
	vulkanTextureImage = std::make_shared<Image>(imageInfo);
	vulkanTextureImage->transitionLayout(vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, commandBuffers);
	textureBuffer->copyFromStagingToDeviceMemory(commandBuffers, vulkanTextureImage);
	vulkanTextureImage->transitionLayout(vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, commandBuffers);
	vulkanTextureImage->createImageView(vk::ImageAspectFlagBits::eColor);
}

const ImageInfo LogicalDevice::createImageInfo(const TextureImage& textureImage, const PhysicalDeviceProperties& physicalDeviceProperties) const
{
	return ImageInfo{
		.vulkanLogicalDevice = vulkanLogicalDevice,
		.physicalDeviceProperties = physicalDeviceProperties,
		.width = textureImage.getWidth(),
		.height = textureImage.getHeight(),
		.format = vk::Format::eR8G8B8A8Srgb,
		.usageFlags = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled
	};
}

void LogicalDevice::createFramebuffers()
{
	swapChain->buildFramebuffers(vulkanLogicalDevice, renderPass->getVulkanRenderPass());
}

void LogicalDevice::createDescriptorSet()
{
	descriptorSet = std::make_unique<DescriptorSet>(vulkanLogicalDevice, MAX_FRAMES_IN_FLIGHT);
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
	graphicsPipelineCreateInfo.vulkanDescriptorSetLayout = descriptorSet->getVulkanDescriptorSetLayout();
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
	descriptorSet->write(uniformBuffers[currentFrame], vulkanTextureImage, currentFrame);
	commandBuffers->record(createCommandBufferRecordInfo(imageIndex));
	commandBuffers->submit(synchronizationObjects[currentFrame], currentFrame);
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