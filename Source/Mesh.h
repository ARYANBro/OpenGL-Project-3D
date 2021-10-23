#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>
#include <memory>

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
	Mesh(const std::string& name) noexcept;
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::string& name) noexcept;
	Mesh() noexcept = default;
	Mesh(Mesh&& other) noexcept;
	void Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) noexcept;

	void SetName(const std::string& name) noexcept { m_Name = name; }

	const VertexArray& GetVertexArray() const noexcept { return m_VertexArray; }
	std::uint_fast32_t GetNumVertices() const noexcept { return m_NumVertices; }
	const std::string& GetName() const noexcept { return m_Name; }

	Mesh& operator=(Mesh&& other) noexcept;

public:
	VertexArray m_VertexArray;
	std::uint_fast32_t m_NumVertices;
	std::string m_Name;
};
