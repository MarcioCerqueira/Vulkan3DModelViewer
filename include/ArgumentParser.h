#pragma once

#include <stdexcept>
#include <fstream>
#include <sstream>

class ArgumentParser
{
public:
	ArgumentParser(int argc, const char* argv[]);
	std::string getConfigurationFile() const noexcept;

private:
	void validateNumberOfArguments(int argc) const;
	void loadConfigurationFile(const char* configurationFilePath);
	std::string configurationFile;
};