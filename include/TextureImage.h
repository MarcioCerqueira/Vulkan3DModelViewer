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
	int getSize() const;
	int getWidth() const;
	int getHeight() const;
	int getMipLevels() const;

private:
	stbi_uc* pixels;
	int width;
	int height;
	int mipLevels;
	int channels;
};