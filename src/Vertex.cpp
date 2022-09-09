#include "Vertex.h"

vk::VertexInputBindingDescription Vertex::getBindingDescription()
{
	return vk::VertexInputBindingDescription{
		.binding = 0,
		.stride = sizeof(Vertex),
		.inputRate = vk::VertexInputRate::eVertex
	};
}

std::array<vk::VertexInputAttributeDescription, 3> Vertex::getAttributeDescriptions()
{
	std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions{};
	attributeDescriptions[0] = getPositionAttributeDescription();
	attributeDescriptions[1] = getColorAttributeDescription();
	attributeDescriptions[2] = getTexCoordAttributeDescription();
	return attributeDescriptions;
}

vk::VertexInputAttributeDescription Vertex::getPositionAttributeDescription()
{
	return vk::VertexInputAttributeDescription{
		.location = 0,
		.binding = 0,
		.format = vk::Format::eR32G32B32Sfloat,
		.offset = offsetof(Vertex, position)
	};
}

vk::VertexInputAttributeDescription Vertex::getColorAttributeDescription()
{
	return vk::VertexInputAttributeDescription{
		.location = 1,
		.binding = 0,
		.format = vk::Format::eR32G32B32Sfloat,
		.offset = offsetof(Vertex, color)
	};
}

vk::VertexInputAttributeDescription Vertex::getTexCoordAttributeDescription()
{
	return vk::VertexInputAttributeDescription{
		.location = 2,
		.binding = 0,
		.format = vk::Format::eR32G32Sfloat,
		.offset = offsetof(Vertex, texCoord)
	};
}