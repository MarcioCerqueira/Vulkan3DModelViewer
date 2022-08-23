#pragma once

#include <memory>
#include "Window.h"
#include "GraphicsInstance.h"
#include "Shader.h"
#include "Scene.h"

class AmbientOcclusionApplication
{

public:
	AmbientOcclusionApplication(const WindowSize& windowSize, Scene& scene);
	void run();

private:
	const std::vector<std::shared_ptr<Shader>> loadShaders(const vk::Device& vulkanLogicalDevice) const;

	const std::string applicationName{ "Ambient Occlusion Application" };
	Window window;
	GraphicsInstance graphicsInstance;
};