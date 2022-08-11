#include "TextureImage.h"

TextureImage::TextureImage(const std::string& filename)
{
	pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	ExceptionChecker::throwExceptionIfPointerIsNull(pixels, "Failed to load texture image!");
}

const stbi_uc* TextureImage::getPixels() const
{
	return pixels;
}

const int TextureImage::getSize() const
{
	return width * height * 4;
}

const int TextureImage::getWidth() const
{
	return width;
}

const int TextureImage::getHeight() const
{
	return height;
}