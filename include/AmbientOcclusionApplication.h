#pragma once

#include <memory>
#include "Window.h"
#include "GraphicsInstance.h"
#include "Shader.h"
#include "GraphicsPipeline.h"
#include "RenderPass.h"

class AmbientOcclusionApplication
{

public:
	AmbientOcclusionApplication(const int windowWidth, const int windowHeight);
	void run() const noexcept;

private:
	void loadGraphicsPipeline();

	std::unique_ptr<Window> window;
	std::unique_ptr<GraphicsInstance> graphicsInstance;
};