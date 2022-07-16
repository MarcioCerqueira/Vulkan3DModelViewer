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
	createCommandBuffer();
	createSynchronizationObjects();
	createQueues(logicalDeviceCreateInfo.queueFamilyIndices);
}

LogicalDevice::~LogicalDevice()
{
	for (auto& framebuffer : framebuffers)
	{
		framebuffer.reset();
	}
	commandPool.reset();
	graphicsPipeline.reset();
	renderPass.reset();
	swapChain.reset();
	synchronizationObjects.reset();
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
	framebuffers.resize(swapChain->getNumberOfImageViews());
	for (int framebufferIndex = 0; framebufferIndex < framebuffers.size(); framebufferIndex++)
	{
		framebuffers[framebufferIndex] = std::make_unique<Framebuffer>(vulkanLogicalDevice, renderPass->getVulkanRenderPass(), swapChain->getImageView(framebufferIndex), swapChain->getExtent());
	}
}

void LogicalDevice::createCommandPool(const std::optional<uint32_t> graphicsFamilyIndex)
{
	commandPool = std::make_unique<CommandPool>(vulkanLogicalDevice, graphicsFamilyIndex);
}

void LogicalDevice::createCommandBuffer()
{
	commandBuffer = std::make_unique<CommandBuffer>(vulkanLogicalDevice, commandPool->getVulkanCommandPool());
}

void LogicalDevice::createSynchronizationObjects()
{
	synchronizationObjects = std::make_shared<SynchronizationObjects>(vulkanLogicalDevice);
}

void LogicalDevice::createQueues(const QueueFamilyIndices& queueFamilyIndices)
{
	graphicsQueue = std::make_unique<GraphicsQueue>(vulkanLogicalDevice, queueFamilyIndices.getGraphicsFamilyIndex());
	presentQueue = std::make_unique<PresentQueue>(vulkanLogicalDevice, queueFamilyIndices.getPresentFamilyIndex());
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
	graphicsPipeline = std::make_unique<GraphicsPipeline>(graphicsPipelineCreateInfo);
}

void LogicalDevice::drawFrame()
{
	const uint32_t fenceCount{ 1 };
	constexpr uint64_t timeout{ std::numeric_limits<uint64_t>::max() };
	vk::Result result{ vulkanLogicalDevice.waitForFences(fenceCount, &synchronizationObjects->inFlight, vk::Bool32{ true }, timeout) };
	ExceptionChecker::throwExceptionIfVulkanResultIsNotSuccess(result, "Failed to wait for fences!");
	result = vulkanLogicalDevice.resetFences(fenceCount, &synchronizationObjects->inFlight);
	ExceptionChecker::throwExceptionIfVulkanResultIsNotSuccess(result, "Failed to reset fences!");
	const uint32_t imageIndex{ swapChain->acquireNextImage(synchronizationObjects->imageAvailable) };
	commandBuffer->reset();
	const vk::RenderPassBeginInfo renderPassBeginInfo{ renderPass->createRenderPassBeginInfo(framebuffers[imageIndex]->getVulkanFramebuffer(), swapChain->getExtent()) };
	commandBuffer->record(renderPassBeginInfo, graphicsPipeline->getVulkanPipeline(), imageIndex);
	graphicsQueue->submit(synchronizationObjects, commandBuffer->getVulkanCommandBuffer(imageIndex));
	presentQueue->presentResult(synchronizationObjects->renderFinished, swapChain->getVulkanSwapChain(), imageIndex);
}

void LogicalDevice::waitIdle()
{
	vulkanLogicalDevice.waitIdle();
}