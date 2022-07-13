#pragma once

#include "Queue.h"
#include "ExceptionChecker.h"

class GraphicsQueue : public Queue
{
public:
	GraphicsQueue(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t>& queueFamilyIndex);
	void submit(vk::Semaphore& imageAvailable, vk::Semaphore& renderFinished, const vk::CommandBuffer& vulkanCommandBuffer, vk::Fence& inFlight);

private:
	vk::SubmitInfo buildSubmitInfo(vk::Semaphore* waitSemaphores, vk::Semaphore* signalSemaphores, vk::PipelineStageFlags* waitStages, const vk::CommandBuffer& vulkanCommandBuffer) const;

};