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
}

void JSONConfigurationFileParser::convertJSONDocumentToConfigurationFileModel() noexcept
{
	configurationFileModel.window.width = JSONDocument["window"].GetObject()["width"].GetInt();
	configurationFileModel.window.height = JSONDocument["window"].GetObject()["height"].GetInt();
}

ConfigurationFileModel JSONConfigurationFileParser::getConfigurationFileModel() const noexcept
{
	return configurationFileModel;
}