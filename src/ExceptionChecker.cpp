#include "ExceptionChecker.h"

void ExceptionChecker::isFileOpen(const std::ifstream& file, const char* path)
{
	if (!file.is_open())
	{
		std::string errorMessage = "Error opening file located at " + std::string(path);
		throw std::runtime_error(errorMessage);
	}
}