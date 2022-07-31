#include "Buffer.h"

Buffer::Buffer(const vk::Device& vulkanLogicalDevice) : vulkanLogicalDevice(vulkanLogicalDevice)
{
}

Buffer::~Buffer()
{
	vulkanLogicalDevice.destroyBuffer(vulkanBuffer);
	vulkanLogicalDevice.freeMemory(vulkanBufferMemory);
	if (isStagingBufferCreated)
	{
		vulkanLogicalDevice.destroyBuffer(vulkanStagingBuffer);
		vulkanLogicalDevice.freeMemory(vulkanStagingBufferMemory);
	}
}

vk::Buffer Buffer::createVulkanBuffer(const vk::DeviceSize& contentSize, const vk::BufferUsageFlags& bufferUsage)
{
	vulkanBufferCreateInfo = buildBufferCreateInfo(contentSize, bufferUsage);
	return vulkanLogicalDevice.createBuffer(vulkanBufferCreateInfo);
}

const vk::BufferCreateInfo Buffer::buildBufferCreateInfo(const vk::DeviceSize& contentSize, const vk::BufferUsageFlags& bufferUsage) const
{
	return vk::BufferCreateInfo{
		.size = contentSize,
		.usage = bufferUsage,
		.sharingMode = vk::SharingMode::eExclusive
	};
}

vk::DeviceMemory Buffer::createVulkanBufferMemory(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::MemoryPropertyFlags& memoryPropertyFlags)
{
	const vk::MemoryRequirements memoryRequirements{ vulkanLogicalDevice.getBufferMemoryRequirements(vulkanBuffer) };
	const uint32_t memoryTypeIndex{ MemoryProperties::findMemoryType(vulkanPhysicalDevice, memoryRequirements.memoryTypeBits, memoryPropertyFlags) };
	const vk::MemoryAllocateInfo memoryAllocateInfo{ buildMemoryAllocateInfo(memoryRequirements, memoryTypeIndex) };
	return vulkanLogicalDevice.allocateMemory(memoryAllocateInfo);
}

const vk::MemoryAllocateInfo Buffer::buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const
{
	return vk::MemoryAllocateInfo{
		.allocationSize = memoryRequirements.size,
		.memoryTypeIndex = memoryTypeIndex
	};
}

void Buffer::bindBufferMemory(vk::Buffer& buffer, vk::DeviceMemory& memory)
{
	const vk::DeviceSize memoryOffset{ 0 };
	vulkanLogicalDevice.bindBufferMemory(buffer, memory, memoryOffset);
}

void Buffer::copyFromStagingToDeviceMemory(const vk::CommandPool& vulkanCommandPool, const std::shared_ptr<GraphicsQueue>& graphicsQueue)
{
	int commandBufferCount = 1;
	CommandBuffer commandBuffer(vulkanLogicalDevice, vulkanCommandPool, commandBufferCount);
	const CommandBufferCopyInfo commandBufferCopyInfo{
		.frameIndex = 0,
		.srcBuffer = vulkanStagingBuffer,
		.dstBuffer = vulkanBuffer,
		.bufferSize = vulkanBufferCreateInfo.size
	};
	commandBuffer.copy(commandBufferCopyInfo);
	graphicsQueue->submit(commandBuffer.getVulkanCommandBuffer(0));
	graphicsQueue->waitIdle();
}

const vk::Buffer Buffer::getVulkanBuffer() const
{
	return vulkanBuffer;
}