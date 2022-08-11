#include "ExceptionChecker.h"

void ExceptionChecker::throwExceptionIfVulkanResultIsNotSuccess(const vk::Result& result, const std::string& errorMessage)
{
	if (result != vk::Result::eSuccess)
	{
		throw std::runtime_error(errorMessage);
	}
}

void ExceptionChecker::throwExceptionIfFileCouldNotBeOpened(const std::ifstream& file, const char* path)
{
	if (!file.is_open())
	{
		std::string errorMessage = "Error opening file located at " + std::string(path);
		throw std::runtime_error(errorMessage);
	}
}

void ExceptionChecker::throwExceptionIfIndexIsOutOfBounds(const int index, const size_t vectorSize, const std::string& errorMessage)
{
	if (index < 0 || index >= vectorSize)
	{
		throw std::runtime_error(errorMessage);
	}
}

void ExceptionChecker::throwExceptionIfPointerIsNull(const void* data, const std::string& errorMessage)
{
	if (!data)
	{
		throw std::runtime_error(errorMessage);
	}
}
