#include "Mesh.h"

Mesh::Mesh(const std::string& name) noexcept
	: m_Name(name)
{
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::string& name) noexcept
	: m_NumVertices(indices.size()), m_Name(name)
{
	Init(vertices, indices);
}

Mesh::Mesh(Mesh&& other) noexcept
{
	*this = std::move(other);
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

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	if (this != &other)
	{
		m_Name = std::move(other.m_Name);
		m_VertexArray = std::move(other.m_VertexArray);
		m_NumVertices = other.m_NumVertices;
	}

	return *this;
}