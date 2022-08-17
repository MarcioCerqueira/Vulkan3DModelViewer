#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

class Vertex
{
public:
	static const vk::VertexInputBindingDescription getBindingDescription();
	static const std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions();
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoord;

private:
	static const vk::VertexInputAttributeDescription getPositionAttributeDescription();
	static const vk::VertexInputAttributeDescription getColorAttributeDescription();
	static const vk::VertexInputAttributeDescription getTexCoordAttributeDescription();
};