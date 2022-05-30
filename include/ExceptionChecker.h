#pragma once

#include <fstream>

class ExceptionChecker
{
public:
	static void isFileOpen(const std::ifstream& file, const char* path);
};