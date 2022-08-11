#pragma once

#include <string>
#include <stdexcept>
#include "stb/stb_image.h"
#include "ExceptionChecker.h"

class TextureImage
{
public:
	explicit TextureImage(const std::string& filename);

	const stbi_uc* getPixels() const;
	const int getSize() const;
	const int getWidth() const;
	const int getHeight() const;

private:
	stbi_uc* pixels;
	int width;
	int height;
	int channels;
};