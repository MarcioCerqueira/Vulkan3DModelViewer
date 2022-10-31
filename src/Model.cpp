#include "Model.h"

Model::Model(const std::string& modelFilename, const std::string& textureFilename) : textureImage(textureFilename)
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
	std::ranges::for_each(shapes, [this, attrib, &uniqueVertices](const auto& shape) {
		std::ranges::for_each(shape.mesh.indices, [this, attrib, &uniqueVertices](const auto& index) {
			const Vertex vertex{
				.position = buildPosition(attrib, index),
				.normal = buildNormal(attrib, index),
				.texCoord = buildTexCoord(attrib, index)
			};
			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		});
	});
}

glm::vec3 Model::buildPosition(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index) const
{
	return glm::vec3{
		attrib.vertices[3 * static_cast<uint32_t>(index.vertex_index) + 0],
		attrib.vertices[3 * static_cast<uint32_t>(index.vertex_index) + 1],
		attrib.vertices[3 * static_cast<uint32_t>(index.vertex_index) + 2]
	};
}

glm::vec3 Model::buildNormal(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index) const
{
	return glm::vec3{
		attrib.normals[3 * static_cast<uint32_t>(index.normal_index) + 0],
		attrib.normals[3 * static_cast<uint32_t>(index.normal_index) + 1],
		attrib.normals[3 * static_cast<uint32_t>(index.normal_index) + 2]
	};
}

glm::vec2 Model::buildTexCoord(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index) const
{
	return glm::vec2{
		attrib.texcoords[2 * static_cast<uint32_t>(index.texcoord_index) + 0],
		1.0 - attrib.texcoords[2 * static_cast<uint32_t>(index.texcoord_index) + 1]
	};
}

const std::vector<Vertex>& Model::getVertices() const
{
	return vertices;
}

const std::vector<uint32_t>& Model::getIndices() const
{
	return indices;
}

const TextureImage& Model::getTextureImage() const
{
	return textureImage;
}
