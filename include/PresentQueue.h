#pragma once

#include "Queue.h"

class PresentQueue : public Queue
{
public:
	PresentQueue(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t>& queueFamilyIndex);
	vk::Result presentResult(vk::Semaphore& renderFinished, const vk::SwapchainKHR& vulkanSwapChain, const uint32_t imageIndex);

private:
	vk::PresentInfoKHR getPresentInfo(vk::Semaphore* signalSemaphores, vk::SwapchainKHR* swapChains, const uint32_t imageIndex) const;

};