#include "AmbientOcclusionApplication.h"

AmbientOcclusionApplication::AmbientOcclusionApplication(int windowWidth, int windowHeight)
{
	window = std::make_unique<Window>(windowWidth, windowHeight);
}

void AmbientOcclusionApplication::run() const noexcept
{
	window->open();
}