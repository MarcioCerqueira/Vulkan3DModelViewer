#pragma once

#include <memory>
#include "Window.h"
#include "GraphicsInstance.h"
#include "Shader.h"

class AmbientOcclusionApplication
{

public:
	AmbientOcclusionApplication(const int windowWidth, const int windowHeight);
	void run() const;

private:
	std::vector<std::shared_ptr<Shader>> loadShaders(const vk::Device& vulkanLogicalDevice) const;

	std::unique_ptr<Window> window;
	std::unique_ptr<GraphicsInstance> graphicsInstance;
};