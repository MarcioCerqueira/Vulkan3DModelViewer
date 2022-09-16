#pragma once

#include "Model.h"
#include "Camera.h"

class Scene
{
public:
	Scene(const std::string& modelFilename, const std::string& textureFilename);
	Model getModel() const;
	Camera& getCamera();

private:
	Model model;
	Camera camera;
};