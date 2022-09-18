#include "Scene.h"

Scene::Scene(const SceneConfigurationModel& sceneConfigurationModel) : model(sceneConfigurationModel.modelFilename, sceneConfigurationModel.textureFilename), camera(sceneConfigurationModel.camera.cameraPosition, sceneConfigurationModel.camera.targetPosition, sceneConfigurationModel.camera.upVector)
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