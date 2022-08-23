#include "Scene.h"

Model Scene::getModel() const
{
	return model;
}

Camera& Scene::getCamera()
{
	return camera;
}