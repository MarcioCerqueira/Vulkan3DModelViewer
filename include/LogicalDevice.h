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
	std::vector<vk::DeviceQueueCreateInfo> buildDeviceQueueCreateInfos(const std::set<uint32_t>& uniqueQueueFamilies) const;
	vk::DeviceQueueCreateInfo buildDeviceQueueCreateInfo(uint32_t queueFamilyIndex) const;
	vk::DeviceCreateInfo buildVulkanLogicalDeviceCreateInfo(const std::vector<vk::DeviceQueueCreateInfo>& deviceQueueCreateInfos, const LogicalDeviceCreateInfo& logicalDeviceCreateInfo) const;
	void createSwapChain(const LogicalDeviceCreateInfo& logicalDeviceCreateInfo);
	void createRenderPass();
	void createFramebuffers();

	vk::Device vulkanLogicalDevice; 
	std::unique_ptr<SwapChain> swapChain;
	std::unique_ptr<RenderPass> renderPass;
	std::unique_ptr<GraphicsPipeline> graphicsPipeline;
	std::vector<std::unique_ptr<Framebuffer>> framebuffers;
	vk::PhysicalDeviceFeatures physicalDeviceFeatures;
};