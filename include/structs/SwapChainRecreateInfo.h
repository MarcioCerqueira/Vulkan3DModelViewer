#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include "WindowSize.h"

struct SwapChainRecreateInfo
{
	vk::RenderPass vulkanRenderPass;
	std::function<WindowSize()> getFramebufferSize;
	std::function<void()> waitEvents;
};