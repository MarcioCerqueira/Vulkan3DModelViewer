#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include "Vertex.h"
#include "TextureImage.h"
#include "tinyobjloader/tiny_obj_loader.h"

class Model
{
public:
	Model(const std::string& modelFilename, const std::string& textureFilename);
	const std::vector<Vertex>& getVertices() const;
	const std::vector<uint32_t>& getIndices() const;
	const TextureImage& getTextureImage() const;

private:
	void buildMesh(const tinyobj::attrib_t& attrib, const std::vector <tinyobj::shape_t>& shapes);
	glm::vec3 buildPosition(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index) const;
	glm::vec3 buildNormal(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index) const;
	glm::vec2 buildTexCoord(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index) const;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	const TextureImage textureImage;
};