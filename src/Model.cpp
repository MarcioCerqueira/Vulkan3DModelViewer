#include "Model.h"

Model::Model() : textureImage(textureFilename)
{
}

const std::vector<Vertex> Model::getVertices() const
{
	return vertices;
}

const std::vector<uint16_t> Model::getIndices() const
{
	return indices;
}

const TextureImage Model::getTextureImage() const
{
	return textureImage;
}
