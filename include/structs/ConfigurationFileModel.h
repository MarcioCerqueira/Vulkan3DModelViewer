#pragma once

#include <string>

struct SceneConfigurationModel
{
	std::string modelFilename;
	std::string textureFilename;
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

