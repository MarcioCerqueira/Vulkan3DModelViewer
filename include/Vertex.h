#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

class Vertex
{
public:
	static const vk::VertexInputBindingDescription getBindingDescription();
	static const std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions();
	bool operator==(const Vertex& other) const 
	{
		return position == other.position && color == other.color && texCoord == other.texCoord;
	}
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoord;

private:
	static const vk::VertexInputAttributeDescription getPositionAttributeDescription();
	static const vk::VertexInputAttributeDescription getColorAttributeDescription();
	static const vk::VertexInputAttributeDescription getTexCoordAttributeDescription();
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}