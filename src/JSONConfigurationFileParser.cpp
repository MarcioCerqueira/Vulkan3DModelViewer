#include "JSONConfigurationFileParser.h"

JSONConfigurationFileParser::JSONConfigurationFileParser(const char* configurationFile)
{
	JSONDocument.Parse(configurationFile);
	validateJSONConfigurationFile();
	convertJSONDocumentToConfigurationFileModel();
}

void JSONConfigurationFileParser::validateJSONConfigurationFile() const
{
	assert(JSONDocument.IsObject());
	assert(JSONDocument.HasMember("window"));
	assert(JSONDocument["window"].HasMember("width"));
	assert(JSONDocument["window"].HasMember("height"));
	assert(JSONDocument["window"].GetObject()["width"].IsInt());
	assert(JSONDocument["window"].GetObject()["height"].IsInt());
	assert(JSONDocument.HasMember("scene"));
	assert(JSONDocument["scene"].HasMember("modelFilename"));
	assert(JSONDocument["scene"].HasMember("textureFilename"));
	assert(JSONDocument["scene"].GetObject()["modelFilename"].IsString());
	assert(JSONDocument["scene"].GetObject()["textureFilename"].IsString());
}

void JSONConfigurationFileParser::convertJSONDocumentToConfigurationFileModel() noexcept
{
	configurationFileModel.window.width = JSONDocument["window"].GetObject()["width"].GetInt();
	configurationFileModel.window.height = JSONDocument["window"].GetObject()["height"].GetInt();
	configurationFileModel.scene.modelFilename = JSONDocument["scene"].GetObject()["modelFilename"].GetString();
	configurationFileModel.scene.textureFilename = JSONDocument["scene"].GetObject()["textureFilename"].GetString();
}

const ConfigurationFileModel& JSONConfigurationFileParser::getConfigurationFileModel() const
{
	return configurationFileModel;
}