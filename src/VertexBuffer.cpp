#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const VertexBufferCreateInfo& vertexBufferCreateInfo) : vulkanLogicalDevice(vertexBufferCreateInfo.vulkanLogicalDevice)
{
	vertexCount = vertexBufferCreateInfo.vertices.size();
	createVertexData(vertexBufferCreateInfo.vertices, vertexBufferCreateInfo.vulkanPhysicalDevice);
	createStagingData(vertexBufferCreateInfo.vertices, vertexBufferCreateInfo.vulkanPhysicalDevice);
	copyFromCPUToStagingMemory(vertexBufferCreateInfo.vertices);
	copyFromStagingToDeviceMemory(vertexBufferCreateInfo.vulkanCommandPool, vertexBufferCreateInfo.graphicsQueue);
}

VertexBuffer::~VertexBuffer()
{
	vulkanLogicalDevice.destroyBuffer(vulkanVertexBuffer);
	vulkanLogicalDevice.destroyBuffer(vulkanStagingBuffer);
	vulkanLogicalDevice.freeMemory(vulkanVertexBufferMemory);
	vulkanLogicalDevice.freeMemory(vulkanStagingBufferMemory);
}

void VertexBuffer::createVertexData(const std::vector<Vertex>& vertices, const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	const vk::BufferUsageFlags vertexBufferUsage{ vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer };
	vulkanVertexBuffer = createVulkanBuffer(vertices, vertexBufferUsage);
	const vk::MemoryPropertyFlags vertexMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eDeviceLocal };
	vulkanVertexBufferMemory = createVulkanBufferMemory(vulkanPhysicalDevice, vertexMemoryPropertyFlags);
	bindBufferMemory(vulkanVertexBuffer, vulkanVertexBufferMemory);
}

void VertexBuffer::createStagingData(const std::vector<Vertex>& vertices, const vk::PhysicalDevice& vulkanPhysicalDevice)
{
	const vk::BufferUsageFlags stagingBufferUsage{ vk::BufferUsageFlagBits::eTransferSrc };
	vulkanStagingBuffer = createVulkanBuffer(vertices, stagingBufferUsage);
	const vk::MemoryPropertyFlags stagingMemoryPropertyFlags{ vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent };
	vulkanStagingBufferMemory = createVulkanBufferMemory(vulkanPhysicalDevice, stagingMemoryPropertyFlags);
	bindBufferMemory(vulkanStagingBuffer, vulkanStagingBufferMemory);
}

vk::Buffer VertexBuffer::createVulkanBuffer(const std::vector<Vertex>& vertices, const vk::BufferUsageFlags& bufferUsage)
{
	vulkanBufferCreateInfo = buildBufferCreateInfo(vertices, bufferUsage);
	return vulkanLogicalDevice.createBuffer(vulkanBufferCreateInfo);
}

const vk::BufferCreateInfo VertexBuffer::buildBufferCreateInfo(const std::vector<Vertex>& vertices, const vk::BufferUsageFlags& bufferUsage) const
{
	return vk::BufferCreateInfo{
		.size = sizeof(vertices[0]) * vertices.size(),
		.usage = bufferUsage,
		.sharingMode = vk::SharingMode::eExclusive
	};
}

vk::DeviceMemory VertexBuffer::createVulkanBufferMemory(const vk::PhysicalDevice& vulkanPhysicalDevice, const vk::MemoryPropertyFlags& memoryPropertyFlags)
{
	const vk::MemoryRequirements memoryRequirements{ vulkanLogicalDevice.getBufferMemoryRequirements(vulkanVertexBuffer) };
	const uint32_t memoryTypeIndex{ MemoryProperties::findMemoryType(vulkanPhysicalDevice, memoryRequirements.memoryTypeBits, memoryPropertyFlags) };
	const vk::MemoryAllocateInfo memoryAllocateInfo{ buildMemoryAllocateInfo(memoryRequirements, memoryTypeIndex) };
	return vulkanLogicalDevice.allocateMemory(memoryAllocateInfo);
}

const vk::MemoryAllocateInfo VertexBuffer::buildMemoryAllocateInfo(const vk::MemoryRequirements& memoryRequirements, const uint32_t memoryTypeIndex) const
{
	return vk::MemoryAllocateInfo{
		.allocationSize = memoryRequirements.size,
		.memoryTypeIndex = memoryTypeIndex
	};
}

void VertexBuffer::bindBufferMemory(vk::Buffer& buffer, vk::DeviceMemory& memory)
{
	const vk::DeviceSize memoryOffset{ 0 };
	vulkanLogicalDevice.bindBufferMemory(buffer, memory, memoryOffset);
}

void VertexBuffer::copyFromCPUToStagingMemory(const std::vector<Vertex>& vertices)
{
	const vk::DeviceSize memoryOffset{ 0 };
	void* data{ vulkanLogicalDevice.mapMemory(vulkanStagingBufferMemory, memoryOffset, vulkanBufferCreateInfo.size) };
	std::memcpy(data, vertices.data(), static_cast<size_t>(vulkanBufferCreateInfo.size));
	vulkanLogicalDevice.unmapMemory(vulkanStagingBufferMemory);
}

void VertexBuffer::copyFromStagingToDeviceMemory(const vk::CommandPool& vulkanCommandPool, const std::shared_ptr<GraphicsQueue>& graphicsQueue)
{
	int commandBufferCount = 1;
	CommandBuffer commandBuffer(vulkanLogicalDevice, vulkanCommandPool, commandBufferCount);
	const CommandBufferCopyInfo commandBufferCopyInfo{
		.frameIndex = 0,
		.srcBuffer = vulkanStagingBuffer,
		.dstBuffer = vulkanVertexBuffer,
		.bufferSize = vulkanBufferCreateInfo.size
	};
	commandBuffer.copy(commandBufferCopyInfo);
	graphicsQueue->submit(commandBuffer.getVulkanCommandBuffer(0));
	graphicsQueue->waitIdle();
}

const vk::Buffer VertexBuffer::getVulkanVertexBuffer() const
{
	return vulkanVertexBuffer;
}

const size_t VertexBuffer::getVertexCount() const
{
	return vertexCount;
}