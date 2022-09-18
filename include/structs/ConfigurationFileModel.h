#pragma once

#include <string>
#include <glm/glm.hpp>

struct CameraConfigurationModel
{
	glm::vec3 cameraPosition;
	glm::vec3 targetPosition;
	glm::vec3 upVector;
};

struct SceneConfigurationModel
{
	std::string modelFilename;
	std::string textureFilename;
	CameraConfigurationModel camera;
};

struct WindowConfigurationModel
{
	int width = 800;
	int height = 600;
};

struct ConfigurationFileModel
{
	WindowConfigurationModel window;
	SceneConfigurationModel scene;
};

