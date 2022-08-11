#pragma once

#include <memory>
#include "Window.h"
#include "GraphicsInstance.h"
#include "Shader.h"
#include "Model.h"

class AmbientOcclusionApplication
{

public:
	AmbientOcclusionApplication(const int windowWidth, const int windowHeight);
	void run();

private:
	const std::vector<std::shared_ptr<Shader>> loadShaders(const vk::Device& vulkanLogicalDevice) const;

	const std::string applicationName{ "Ambient Occlusion Application" };
	Window window;
	GraphicsInstance graphicsInstance;
	Model model;
};