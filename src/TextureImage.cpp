#include "TextureImage.h"

TextureImage::TextureImage(const std::string& filename)
{
	pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	mipLevels = static_cast<int>(std::floor(std::log2(std::max(width, height)))) + 1;
	ExceptionChecker::throwExceptionIfPointerIsNull(pixels, "Failed to load texture image!");
}

const stbi_uc* TextureImage::getPixels() const
{
	return pixels;
}

int TextureImage::getSize() const
{
	return width * height * 4;
}

int TextureImage::getWidth() const
{
	return width;
}

int TextureImage::getHeight() const
{
	return height;
}

int TextureImage::getMipLevels() const
{
	return mipLevels;
}