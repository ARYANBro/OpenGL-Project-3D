#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) noexcept
	: m_NumVertices(indices.size())
{
	Init(vertices, indices);
}

void Mesh::Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) noexcept
{
	m_NumVertices = indices.size();
	AttributeFormat attribFormat0 = {
		.Size = 3,
		.Type = GL_FLOAT,
		.Normalized = false,
		.RelativeOffset = 0
	};

	AttributeFormat attribFormat1 = {
		.Size = 2,
		.Type = GL_FLOAT,
		.Normalized = false,
		.RelativeOffset = offsetof(Vertex, Vertex::TextureCoord)
	};

	AttributeFormat attribFormat2 = {
		.Size = 3,
		.Type = GL_FLOAT,
		.Normalized = false,
		.RelativeOffset = offsetof(Vertex, Vertex::Normal)
	};

	AttributeFormat attribFormat3 = {
		.Size = 3,
		.Type = GL_FLOAT,
		.Normalized = false,
		.RelativeOffset = offsetof(Vertex, Vertex::Tangent)
	};

	auto vertexBuffer = std::make_unique<Buffer>(vertices.data(), vertices.size() * sizeof(Vertex));
	m_VertexArray.BindVertexBuffer(0, std::move(vertexBuffer), sizeof(Vertex));
	
	m_VertexArray.EnableAttribute(0);
	m_VertexArray.EnableAttribute(1);
	m_VertexArray.EnableAttribute(2);
	m_VertexArray.EnableAttribute(3);

	m_VertexArray.BindAttribute(0, 0);
	m_VertexArray.BindAttribute(1, 0);
	m_VertexArray.BindAttribute(2, 0);
	m_VertexArray.BindAttribute(3, 0);

	m_VertexArray.SetAttributeFormat(0, attribFormat0);
	m_VertexArray.SetAttributeFormat(1, attribFormat1);
	m_VertexArray.SetAttributeFormat(2, attribFormat2);
	m_VertexArray.SetAttributeFormat(3, attribFormat3);

	auto indexBuffer = std::make_unique<Buffer>(indices.data(), indices.size() * sizeof(unsigned int));
	m_VertexArray.BindIndexBuffer(std::move(indexBuffer));
}

std::vector<glm::vec3> Mesh::CalculateTangents(const std::vector<unsigned int>& indices, const std::vector<glm::vec3>& vertexPos, const std::vector<glm::vec2>& textureCoords) noexcept
{
	std::vector<glm::vec3> tangents;
	tangents.reserve(indices.size() * 2);

	for (std::size_t i = 0; i < indices.size(); i+= 3)
	{
		glm::vec3 vertexPos0 = vertexPos[indices[i]];
		glm::vec3 vertexPos1 = vertexPos[indices[i + 1]];
		glm::vec3 vertexPos2 = vertexPos[indices[i + 2]];

		glm::vec2 uv0 = textureCoords[indices[i]];
		glm::vec2 uv1 = textureCoords[indices[i + 1]];
		glm::vec2 uv2 = textureCoords[indices[i + 2]];

		glm::vec3 edge1 = vertexPos1 - vertexPos0;
		glm::vec3 edge2 = vertexPos2 - vertexPos0;
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);

		tangents.push_back(tangent);
		tangents.push_back(tangent);
	}

	return tangents;
}

std::vector<Vertex> Mesh::CalculateVertices(const std::vector<glm::vec3>& vertexPos, const std::vector<glm::vec2>& textureCoords, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& tangents) noexcept
{
	std::size_t vertexLength = std::size(vertexPos);

	std::vector<Vertex> vertices(vertexLength);

	for (int i = 0; i < vertexLength; i++)
	{
		vertices[i].VertexPos = vertexPos[i];
		vertices[i].Normal = normals[i];
		vertices[i].TextureCoord = textureCoords[i];
		vertices[i].Tangent = tangents[i];
	}

	return vertices;
}