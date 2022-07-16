#include "GraphicsQueue.h"

GraphicsQueue::GraphicsQueue(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t>& queueFamilyIndex) : Queue(vulkanLogicalDevice, queueFamilyIndex)
{
}

void GraphicsQueue::submit(std::shared_ptr<SynchronizationObjects>& synchronizationObjects, const vk::CommandBuffer& vulkanCommandBuffer)
{
	vk::Semaphore waitSemaphores[] = { synchronizationObjects->imageAvailable };
	vk::Semaphore signalSemaphores[] = { synchronizationObjects->renderFinished };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::SubmitInfo submitInfo{ buildSubmitInfo(waitSemaphores, signalSemaphores, waitStages, vulkanCommandBuffer) };
	vulkanQueue.submit(submitInfo, synchronizationObjects->inFlight);
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