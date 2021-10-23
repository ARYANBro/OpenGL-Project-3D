#pragma once

#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>

#include <vector>
#include <functional>

class Material
{
public:
	Material(std::uint_fast32_t shaderID) noexcept
		: m_ShaderID(shaderID) {}

	Material(const Material&) noexcept = default;

	void Bind() const noexcept;
	void Unbind() const noexcept;
	void UpdateUniforms() const noexcept;

	void SetDiffuseTexture(std::uint_fast32_t textureID) noexcept { m_DiffuseTexID = textureID; }
	void SetSpecularTexture(std::uint_fast32_t textureID) noexcept { m_SpecularTexID = textureID; }
	void SetNormalMap(std::uint_fast32_t textureID) noexcept { m_NormalMapID = textureID; }
	void SetSpecularExponent(float exponent) noexcept { m_SpecularExp = exponent; }
	void SetDiffuse(glm::vec3 diffuse) noexcept { m_Diffuse = diffuse; }
	void SetSpecular(glm::vec3 specular) noexcept { m_Specular = specular; }
	void SetAmbient(glm::vec3 ambient) noexcept { m_Ambient = ambient; }

	std::uint_fast32_t GetShaderProgram() const noexcept { return m_ShaderID; }
	std::uint_fast32_t GetDiffuseTextureID() const noexcept { return m_DiffuseTexID; }
	std::uint_fast32_t GetSpecularTextureID() const noexcept { return m_SpecularTexID; }
	std::uint_fast32_t GetNormalMapID() const noexcept { return m_NormalMapID; }
	float GetSpecularExponent() const noexcept { return m_SpecularExp; }
	glm::vec3 GetDiffuse() const noexcept { return m_Diffuse; }
	glm::vec3 GetSpecular() const noexcept { return m_Specular; }
	glm::vec3 GetAmbient() const noexcept { return m_Ambient; }

	Material& operator=(Material&) noexcept = default;

private:
	std::uint_fast32_t m_ShaderID = 0;

	std::uint_fast32_t m_DiffuseTexID = 0;
	std::uint_fast32_t m_SpecularTexID = 0;
	std::uint_fast32_t m_NormalMapID = 0;

	float m_SpecularExp;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	glm::vec3 m_Ambient;
};