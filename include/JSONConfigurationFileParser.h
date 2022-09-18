#pragma once

#include "rapidjson/document.h"
#include "structs/ConfigurationFileModel.h"

class JSONConfigurationFileParser
{
public:
	explicit JSONConfigurationFileParser(const char* configurationFile);
	const ConfigurationFileModel& getConfigurationFileModel() const;

private:
	void validateJSONConfigurationFile() const;
	void assertJSONObjectIsA3DVector(const rapidjson::Value& JSONObject) const;
	void convertJSONDocumentToConfigurationFileModel();
	glm::vec3 convertJSONObjectTo3DVector(const rapidjson::Value& JSONObject) const;

	rapidjson::Document JSONDocument;
	ConfigurationFileModel configurationFileModel;
};
