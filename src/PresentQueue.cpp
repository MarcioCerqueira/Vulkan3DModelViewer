#include "PresentQueue.h"

PresentQueue::PresentQueue(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t>& queueFamilyIndex) : Queue(vulkanLogicalDevice, queueFamilyIndex)
{

}

void PresentQueue::presentResult(vk::Semaphore& renderFinished, const vk::SwapchainKHR& vulkanSwapChain, const uint32_t imageIndex)
{
	vk::Semaphore signalSemaphores[] = { renderFinished };
	vk::SwapchainKHR swapChains[] = { vulkanSwapChain };
    vk::PresentInfoKHR presentInfo{ getPresentInfo(signalSemaphores, swapChains, imageIndex) };
	const vk::Result result{ vulkanQueue.presentKHR(presentInfo) };
	ExceptionChecker::throwExceptionIfVulkanResultIsNotSuccess(result, "Failed to present the results!");
}

vk::PresentInfoKHR PresentQueue::getPresentInfo(vk::Semaphore* signalSemaphores, vk::SwapchainKHR* swapChains, const uint32_t imageIndex) const
{
    return vk::PresentInfoKHR{
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signalSemaphores,
        .swapchainCount = 1,
        .pSwapchains = swapChains,
        .pImageIndices = &imageIndex,
        .pResults = nullptr
    };
}
