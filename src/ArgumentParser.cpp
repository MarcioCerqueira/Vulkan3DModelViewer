#include "ArgumentParser.h"

ArgumentParser::ArgumentParser(const int argc, const char* argv[])
{
	validateNumberOfArguments(argc);
	loadConfigurationFile(argv[1]);
}

const std::string& ArgumentParser::getConfigurationFile() const
{
	return configurationFile;
}

void ArgumentParser::validateNumberOfArguments(const int argc) const
{
	if (argc != 2)
	{
		throw std::runtime_error("Error! You need to pass the configuration file as an argument (e.g., main.exe config.json)");
	}
}

void ArgumentParser::loadConfigurationFile(const char* configurationFilePath)
{
	const std::ifstream file{ openFile(configurationFilePath) };
	std::stringstream buffer;
	buffer << file.rdbuf();
	configurationFile = buffer.str();
}

std::ifstream ArgumentParser::openFile(const char* filePath) const
{
	std::ifstream file(filePath);
	ExceptionChecker::throwExceptionIfFileCouldNotBeOpened(file, filePath);
	return file;
}