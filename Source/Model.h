#pragma once

#include "Mesh.h"
#include "Material.h"

#include <glm/glm.hpp>

#include <unordered_map>
#include <memory>

class Model
{
public:
	Model(Mesh&& mesh, const Material& material, const glm::mat4& transform = glm::mat4(1.0f)) noexcept
		: m_Mesh(std::move(mesh)), m_Material(material), m_Transform(transform) {}

	Model(Model&& model) noexcept;

	void SetTransform(const glm::mat4& transform) noexcept { m_Transform = transform; }

	const Mesh& GetMesh() const noexcept { return /* * */m_Mesh; }
	const Material& GetMaterial() const noexcept { return m_Material; }
	const glm::mat4& GetTransform() const noexcept { return m_Transform; }

	Model& operator=(Model&& other) noexcept;

private:
	Mesh m_Mesh;
	Material m_Material;
	glm::mat4 m_Transform;
};
