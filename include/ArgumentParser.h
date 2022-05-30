#pragma once

#include <stdexcept>
#include <fstream>
#include <sstream>

#include "ExceptionChecker.h"

class ArgumentParser
{
public:
	ArgumentParser(const int argc, const char* argv[]);
	std::string getConfigurationFile() const noexcept;

private:
	void validateNumberOfArguments(const int argc) const;
	void loadConfigurationFile(const char* filePath);
	std::ifstream openFile(const char* configurationFilePath) const;

	std::string configurationFile;
};