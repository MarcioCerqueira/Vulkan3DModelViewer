#pragma once

#include "rapidjson/document.h"
#include "ConfigurationFileModel.h"

class JSONConfigurationFileParser
{
public:
	explicit JSONConfigurationFileParser(const char* configurationFile);
	ConfigurationFileModel getConfigurationFileModel() const noexcept;

private:
	void validateJSONConfigurationFile() const;
	void convertJSONDocumentToConfigurationFileModel() noexcept;

	rapidjson::Document JSONDocument;
	ConfigurationFileModel configurationFileModel;
};
