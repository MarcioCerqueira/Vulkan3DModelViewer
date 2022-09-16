#include "Scene.h"

Scene::Scene(const std::string& modelFilename, const std::string& textureFilename) : model(modelFilename, textureFilename)
{
}

Model Scene::getModel() const
{
	return model;
}

Camera& Scene::getCamera()
{
	return camera;
}