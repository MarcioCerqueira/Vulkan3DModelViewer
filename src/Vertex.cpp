#include "Vertex.h"

const vk::VertexInputBindingDescription Vertex::getBindingDescription()
{
	return vk::VertexInputBindingDescription{
		.binding = 0,
		.stride = sizeof(Vertex),
		.inputRate = vk::VertexInputRate::eVertex
	};
}

const std::array<vk::VertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions()
{
	std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions{};
	attributeDescriptions[0] = getPositionAttributeDescription();
	attributeDescriptions[1] = getColorAttributeDescription();
	return attributeDescriptions;
}

const vk::VertexInputAttributeDescription Vertex::getPositionAttributeDescription()
{
	return vk::VertexInputAttributeDescription{
		.location = 0,
		.binding = 0,
		.format = vk::Format::eR32G32Sfloat,
		.offset = offsetof(Vertex, position)
	};
}

const vk::VertexInputAttributeDescription Vertex::getColorAttributeDescription()
{
	return vk::VertexInputAttributeDescription{
		.location = 1,
		.binding = 0,
		.format = vk::Format::eR32G32B32Sfloat,
		.offset = offsetof(Vertex, color)
	};
}