#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

#include <vector>

struct Vertex
{
	glm::vec3 VertexPos;
	glm::vec2 TextureCoord;
	glm::vec3 Normal;
	glm::vec3 Tangent;
};

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) noexcept;
	Mesh() noexcept = default;
	void Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) noexcept;

	const VertexArray& GetVertexArray() const noexcept { return m_VertexArray; }
	std::uint_fast32_t GetNumVertices() const noexcept { return m_NumVertices; }

	static std::vector<glm::vec3> CalculateTangents(const std::vector<unsigned int>& indices, const std::vector<glm::vec3>& vertexPos, const std::vector<glm::vec2>& textureCoords) noexcept;
	static std::vector<Vertex> CalculateVertices(const std::vector<glm::vec3>& vertexPos, const std::vector<glm::vec2>& textureCoords, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& tangents) noexcept;

public:
	VertexArray m_VertexArray;
	std::uint_fast32_t m_NumVertices;
};	
