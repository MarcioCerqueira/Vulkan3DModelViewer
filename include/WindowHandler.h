#pragma once

#include "structs/WindowSize.h"

class WindowHandler
{
public:
	virtual WindowSize getFramebufferSize() const = 0;
	virtual void waitEvents() const = 0;
};