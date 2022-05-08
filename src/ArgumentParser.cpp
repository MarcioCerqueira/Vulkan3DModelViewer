#include "ArgumentParser.h"

ArgumentParser::ArgumentParser(int argc, const char* argv[])
{
	validateNumberOfArguments(argc);
	loadConfigurationFile(argv[1]);
}

std::string ArgumentParser::getConfigurationFile() const noexcept
{
	return configurationFile;
}

void ArgumentParser::validateNumberOfArguments(int argc) const
{
	if (argc != 2)
	{
		throw std::runtime_error("Error! You need to pass the configuration file as an argument (e.g., main.exe config.json)");
	}
}

void ArgumentParser::loadConfigurationFile(const char* configurationFilePath)
{
	std::ifstream file = openFile(configurationFilePath);
	std::stringstream buffer;
	buffer << file.rdbuf();
	configurationFile = buffer.str();
}

std::ifstream ArgumentParser::openFile(const char* configurationFilePath) const
{
	std::ifstream file(configurationFilePath);
	if (!file.is_open())
	{
		std::string errorMessage = "Error opening file located at " + std::string(configurationFilePath);
		throw std::runtime_error(errorMessage);
	}
	return file;
}