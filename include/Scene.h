#pragma once

#include "Model.h"
#include "Camera.h"
#include "structs\ConfigurationFileModel.h"

class Scene
{
public:
	Scene(const SceneConfigurationModel& sceneConfigurationModel);
	Model getModel() const;
	Camera& getCamera();

private:
	Model model;
	Camera camera;
};