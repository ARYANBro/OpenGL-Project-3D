#include "Model.h"

Model::Model(Model&& other) noexcept
	: m_Material(-1)
{
	*this = std::move(other);
}

Model& Model::operator=(Model&& other) noexcept
{
	if (this != &other)
	{
		m_Mesh = std::move(other.m_Mesh);
		m_Material = other.m_Material;
		m_Transform = other.m_Transform;
	}

	return *this;
}