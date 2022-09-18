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
	assert(JSONDocument["window"]["width"].IsInt());
	assert(JSONDocument["window"]["height"].IsInt());
	assert(JSONDocument.HasMember("scene"));
	assert(JSONDocument["scene"].HasMember("modelFilename"));
	assert(JSONDocument["scene"].HasMember("textureFilename"));
	assert(JSONDocument["scene"].HasMember("camera"));
	assert(JSONDocument["scene"]["modelFilename"].IsString());
	assert(JSONDocument["scene"]["textureFilename"].IsString());
	assert(JSONDocument["scene"]["camera"].HasMember("position"));
	assert(JSONDocument["scene"]["camera"].HasMember("target"));
	assert(JSONDocument["scene"]["camera"].HasMember("up"));
	assertJSONObjectIsA3DVector(JSONDocument["scene"]["camera"]["position"]);
	assertJSONObjectIsA3DVector(JSONDocument["scene"]["camera"]["target"]);
	assertJSONObjectIsA3DVector(JSONDocument["scene"]["camera"]["up"]);
}

void JSONConfigurationFileParser::assertJSONObjectIsA3DVector(const rapidjson::Value& JSONObject) const
{
	assert(JSONObject.HasMember("x"));
	assert(JSONObject.HasMember("y"));
	assert(JSONObject.HasMember("z"));
	assert(JSONObject["x"].IsFloat());
	assert(JSONObject["y"].IsFloat());
	assert(JSONObject["z"].IsFloat());
}

void JSONConfigurationFileParser::convertJSONDocumentToConfigurationFileModel()
{
	configurationFileModel.window.width = JSONDocument["window"]["width"].GetInt();
	configurationFileModel.window.height = JSONDocument["window"]["height"].GetInt();
	configurationFileModel.scene.modelFilename = JSONDocument["scene"]["modelFilename"].GetString();
	configurationFileModel.scene.textureFilename = JSONDocument["scene"]["textureFilename"].GetString();
	configurationFileModel.scene.camera.cameraPosition = convertJSONObjectTo3DVector(JSONDocument["scene"]["camera"]["position"]);
	configurationFileModel.scene.camera.targetPosition = convertJSONObjectTo3DVector(JSONDocument["scene"]["camera"]["target"]);
	configurationFileModel.scene.camera.upVector = convertJSONObjectTo3DVector(JSONDocument["scene"]["camera"]["up"]);
}

glm::vec3 JSONConfigurationFileParser::convertJSONObjectTo3DVector(const rapidjson::Value& JSONObject) const
{
	return glm::vec3{ JSONObject["x"].GetFloat(), JSONObject["y"].GetFloat(), JSONObject["z"].GetFloat() };
}

const ConfigurationFileModel& JSONConfigurationFileParser::getConfigurationFileModel() const
{
	return configurationFileModel;
}