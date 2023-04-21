#pragma once

#include <memory>
#include "Window.h"
#include "GraphicsInstance.h"
#include "Shader.h"
#include "Scene.h"

class Application
{

public:
	Application(const WindowSize& windowSize, Scene& scene);
	void run();

private:
	std::vector<std::shared_ptr<Shader>> loadShaders(const vk::Device& vulkanLogicalDevice) const;

	const std::string applicationName{ "Application" };
	Window window;
	GraphicsInstance graphicsInstance;
};