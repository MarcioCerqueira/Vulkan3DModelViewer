#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

class SynchronizationObjects
{

public:
	explicit SynchronizationObjects(const vk::Device& vulkanLogicalDevice);
	~SynchronizationObjects();
	
	vk::Semaphore imageAvailable;
	vk::Semaphore renderFinished;
	vk::Fence inFlight;

private:
	const vk::Device vulkanLogicalDevice;

};