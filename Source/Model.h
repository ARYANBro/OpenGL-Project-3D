#pragma once

#include "Mesh.h"
#include "Material.h"

#include <glm/glm.hpp>

#include <unordered_map>
#include <memory>

class Model
{
public:
	Model(const std::string& name) noexcept : m_Name(name) {}
	Model(const std::shared_ptr<Material>& material, const std::string& name) noexcept : m_Material(material), m_Name(name) { }
	Model(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material, const glm::mat4& transform, const std::string& name) noexcept
		: m_Mesh(mesh), m_Material(material), m_Transform(transform), m_Name(name) {}

	void SetMesh(const std::shared_ptr<Mesh>& mesh) noexcept { m_Mesh = mesh; }
	void SetTransform(const glm::mat4& transform) noexcept { m_Transform = transform; }
	void SetName(const std::string& name) noexcept { m_Name = name; }
	void SetMaterial(const std::shared_ptr<Material>& material) noexcept { m_Material = material; }

	const Mesh& GetMesh() const noexcept { return *m_Mesh; }
	const Material& GetMaterial() const noexcept { return *m_Material; }
	const glm::mat4& GetTransform() const noexcept { return m_Transform; }
	const std::string& GetName() const noexcept { return m_Name; }

private:
	std::shared_ptr<Mesh> m_Mesh;
	std::shared_ptr<Material> m_Material;
	glm::mat4 m_Transform;
	std::string m_Name;
};
