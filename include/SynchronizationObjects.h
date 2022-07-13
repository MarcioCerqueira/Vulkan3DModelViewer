#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

struct SynchronizationObjects
{
	vk::Semaphore imageAvailable;
	vk::Semaphore renderFinished;
	vk::Fence inFlight;
};