#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <fstream>

class ExceptionChecker
{
public:
	static void throwExceptionIfVulkanResultIsNotSuccess(const vk::Result& result, const std::string& errorMessage);
	static void throwExceptionIfFileCouldNotBeOpened(const std::ifstream& file, const char* path);
	static void throwExceptionIfIndexIsOutOfBounds(const int index, const size_t vectorSize, const std::string& errorMessage);
};