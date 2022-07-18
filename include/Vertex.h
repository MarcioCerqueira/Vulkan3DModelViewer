#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

class Vertex
{
public:
	static const vk::VertexInputBindingDescription getBindingDescription();
	static const std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions();
	glm::vec2 position;
	glm::vec3 color;

private:
	static const vk::VertexInputAttributeDescription getPositionAttributeDescription();
	static const vk::VertexInputAttributeDescription getColorAttributeDescription();
};