#pragma once
#include <glm/glm.hpp>

struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 projection;
	alignas(16) glm::vec4 cameraPosition;
	alignas(16) float showTexture;
};