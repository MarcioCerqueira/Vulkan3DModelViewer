#include "Model.h"

Model::Model() : textureImage(textureFilename)
{
	tinyobj::attrib_t attrib;
	std::vector < tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelFilename.c_str())) 
	{
		throw std::runtime_error(warn + err);
	}
	buildMesh(attrib, shapes);
}

void Model::buildMesh(const tinyobj::attrib_t& attrib, const std::vector <tinyobj::shape_t>& shapes)
{
	std::unordered_map<Vertex, uint32_t> uniqueVertices;
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			const Vertex vertex{
				.position = buildPosition(attrib, index),
				.color = {1.0, 1.0f, 1.0f},
				.texCoord = buildTexCoord(attrib, index)
			};
			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

const glm::vec3 Model::buildPosition(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index) const
{
	return glm::vec3{
		attrib.vertices[3 * static_cast<uint32_t>(index.vertex_index) + 0],
		attrib.vertices[3 * static_cast<uint32_t>(index.vertex_index) + 1],
		attrib.vertices[3 * static_cast<uint32_t>(index.vertex_index) + 2]
	};
}

const glm::vec2 Model::buildTexCoord(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index) const
{
	return glm::vec2{
		attrib.texcoords[2 * static_cast<uint32_t>(index.texcoord_index) + 0],
		1.0 - attrib.texcoords[2 * static_cast<uint32_t>(index.texcoord_index) + 1]
	};
}

const std::vector<Vertex> Model::getVertices() const
{
	return vertices;
}

const std::vector<uint32_t> Model::getIndices() const
{
	return indices;
}

const TextureImage Model::getTextureImage() const
{
	return textureImage;
}
