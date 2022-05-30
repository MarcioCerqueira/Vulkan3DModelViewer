#pragma once

#include <memory>

#include "Window.h"
#include "GraphicInstance.h"
#include "WindowSurface.h"
#include "Shader.h"

class AmbientOcclusionApplication
{

public:
	AmbientOcclusionApplication(const int windowWidth, const int windowHeight);
	void run() const noexcept;

private:
	void loadGraphicsPipeline();

	std::unique_ptr<Window> window;
	std::unique_ptr<GraphicInstance> graphicInstance;
	std::unique_ptr<WindowSurface> windowSurface;
};