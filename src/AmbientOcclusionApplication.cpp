#include "AmbientOcclusionApplication.h"

AmbientOcclusionApplication::AmbientOcclusionApplication(const int windowWidth, const int windowHeight)
{
	const std::string applicationName{ "Ambient Occlusion Application" };
	window = std::make_unique<Window>(windowWidth, windowHeight, applicationName);
	graphicInstance = std::make_unique<GraphicInstance>(applicationName);
}

void AmbientOcclusionApplication::run() const noexcept
{
	window->open();
}