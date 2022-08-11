#pragma once

#include <vector>
#include <string>
#include "Vertex.h"
#include "TextureImage.h"

class Model
{
public:
	Model();
	const std::vector<Vertex> getVertices() const;
	const std::vector<uint16_t> getIndices() const;
	const TextureImage getTextureImage() const;

private:
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};
	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};
	const std::string textureFilename{ "texture.jpg" };
	const TextureImage textureImage;
};