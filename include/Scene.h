#pragma once

#include "Model.h"
#include "Camera.h"

class Scene
{
public:
	Model getModel() const;
	Camera& getCamera();

private:
	Model model;
	Camera camera;
};