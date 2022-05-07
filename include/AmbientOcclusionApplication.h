#pragma once

#include <memory>

#include "Window.h"

class AmbientOcclusionApplication
{

public:
	AmbientOcclusionApplication(int windowWidth, int windowHeight);
	void run() const noexcept;

private:
	std::unique_ptr<Window> window;

};