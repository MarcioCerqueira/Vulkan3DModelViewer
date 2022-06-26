#include "LogicalDevice.h"

LogicalDevice::LogicalDevice(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo)
{
	const std::set<uint32_t> uniqueQueueFamilies = { 
		logicalDeviceCreateInfo.queueFamilyIndices.getGraphicsFamilyIndex().value(),
		logicalDeviceCreateInfo.queueFamilyIndices.getPresentFamilyIndex().value()
	};
	const std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfos{ buildDeviceQueueCreateInfos(uniqueQueueFamilies) };
	const vk::DeviceCreateInfo vulkanLogicalDeviceCreateInfo{ buildVulkanLogicalDeviceCreateInfo(deviceQueueCreateInfos, logicalDeviceCreateInfo) };
	vulkanLogicalDevice = logicalDeviceCreateInfo.vulkanPhysicalDevice.createDevice(vulkanLogicalDeviceCreateInfo);
	createSwapChain(logicalDeviceCreateInfo);
	createRenderPass();
	createFramebuffers();
}

LogicalDevice::~LogicalDevice()
{
	for (auto& framebuffer : framebuffers)
	{
		framebuffer.reset();
	}
	graphicsPipeline.reset();
	renderPass.reset();
	swapChain.reset();
	vulkanLogicalDevice.destroy();
}

std::vector<vk::DeviceQueueCreateInfo> LogicalDevice::buildDeviceQueueCreateInfos(const std::set<uint32_t>& uniqueQueueFamilies) const
{
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
	{
		queueCreateInfos.push_back(buildDeviceQueueCreateInfo(queueFamilyIndex));
	}
	return queueCreateInfos;
}

vk::DeviceQueueCreateInfo LogicalDevice::buildDeviceQueueCreateInfo(uint32_t queueFamilyIndex) const
{
	const float queuePriority{ 1.0f };
	return vk::DeviceQueueCreateInfo{
		.queueFamilyIndex = queueFamilyIndex,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority
	};
}

vk::DeviceCreateInfo LogicalDevice::buildVulkanLogicalDeviceCreateInfo(const std::vector<vk::DeviceQueueCreateInfo>& deviceQueueCreateInfos, const LogicalDeviceCreateInfo& logicalDeviceCreateInfo) const
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

vk::Device LogicalDevice::getVulkanLogicalDevice() const noexcept
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