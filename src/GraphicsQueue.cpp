#include "GraphicsQueue.h"

GraphicsQueue::GraphicsQueue(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t>& queueFamilyIndex) : Queue(vulkanLogicalDevice, queueFamilyIndex)
{
}

void GraphicsQueue::submit(vk::Semaphore& imageAvailable, vk::Semaphore& renderFinished, const vk::CommandBuffer& vulkanCommandBuffer, vk::Fence& inFlight)
{
	vk::Semaphore waitSemaphores[] = { imageAvailable };
	vk::Semaphore signalSemaphores[] = { renderFinished };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::SubmitInfo submitInfo{ buildSubmitInfo(waitSemaphores, signalSemaphores, waitStages, vulkanCommandBuffer) };
	vulkanQueue.submit(submitInfo, inFlight);
}

vk::SubmitInfo GraphicsQueue::buildSubmitInfo(vk::Semaphore* waitSemaphores, vk::Semaphore* signalSemaphores, vk::PipelineStageFlags* waitStages, const vk::CommandBuffer& vulkanCommandBuffer) const
{
	return vk::SubmitInfo{
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = waitSemaphores,
		.pWaitDstStageMask = waitStages,
		.commandBufferCount = 1,
		.pCommandBuffers = &vulkanCommandBuffer,
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = signalSemaphores
	};
}