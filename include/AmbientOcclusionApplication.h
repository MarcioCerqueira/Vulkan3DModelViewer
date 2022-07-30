#pragma once

#include <memory>
#include "Window.h"
#include "GraphicsInstance.h"
#include "Shader.h"
#include "Vertex.h"

class AmbientOcclusionApplication
{

public:
	AmbientOcclusionApplication(const int windowWidth, const int windowHeight);
	~AmbientOcclusionApplication();
	void run();

private:
	const std::vector<std::shared_ptr<Shader>> loadShaders(const vk::Device& vulkanLogicalDevice) const;

	std::shared_ptr<Window> window;
	std::shared_ptr<GraphicsInstance> graphicsInstance;
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};
	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};
};