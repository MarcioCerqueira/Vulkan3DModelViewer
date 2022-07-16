#include "SynchronizationObjects.h"

SynchronizationObjects::SynchronizationObjects(const vk::Device& vulkanLogicalDevice) : vulkanLogicalDevice(vulkanLogicalDevice)
{
	vk::FenceCreateInfo fenceCreateInfo{ .flags = vk::FenceCreateFlagBits::eSignaled };
	imageAvailable = vulkanLogicalDevice.createSemaphore({});
	renderFinished = vulkanLogicalDevice.createSemaphore({});
	inFlight = vulkanLogicalDevice.createFence(fenceCreateInfo);
}

SynchronizationObjects::~SynchronizationObjects()
{
	vulkanLogicalDevice.destroySemaphore(imageAvailable);
	vulkanLogicalDevice.destroySemaphore(renderFinished);
	vulkanLogicalDevice.destroyFence(inFlight);
}