#pragma once

#include "Queue.h"
#include "ExceptionChecker.h"
#include "SynchronizationObjects.h"

class GraphicsQueue : public Queue
{
public:
	GraphicsQueue(const vk::Device& vulkanLogicalDevice, const std::optional<uint32_t>& queueFamilyIndex);
	void submit(std::shared_ptr<SynchronizationObjects>& synchronizationObjects, const vk::CommandBuffer& vulkanCommandBuffer);

private:
	vk::SubmitInfo buildSubmitInfo(vk::Semaphore* waitSemaphores, vk::Semaphore* signalSemaphores, vk::PipelineStageFlags* waitStages, const vk::CommandBuffer& vulkanCommandBuffer) const;

};