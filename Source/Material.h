#pragma once

#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>

class Material
{
public:
	Material(const std::string& name) noexcept
		: m_Name(name) {}

	Material(const std::shared_ptr<Shader>& shader, const std::string& name) noexcept
		: m_Shader(shader), m_Name(name) {}

	void Bind() const noexcept;
	void Unbind() const noexcept;
	void UpdateUniforms() const noexcept;

	void SetShaderProgram(const std::shared_ptr<Shader>& shader) noexcept { m_Shader = shader; }
	void SetDiffuseTexture(const std::shared_ptr<Texture>& texture) noexcept { m_DiffuseTex = texture; }
	void SetSpecularTexture(const std::shared_ptr<Texture>& texture) noexcept { m_SpecularTex = texture; }
	void SetNormalMap(const std::shared_ptr<Texture>& texture) noexcept { m_NormalMap = texture; }
	void SetSpecularExponent(float exponent) noexcept { m_SpecularExp = exponent; }
	void SetDiffuse(glm::vec3 diffuse) noexcept { m_Diffuse = diffuse; }
	void SetSpecular(glm::vec3 specular) noexcept { m_Specular = specular; }
	void SetAmbient(glm::vec3 ambient) noexcept { m_Ambient = ambient; }

	const Shader& GetShaderProgram() const noexcept { return *m_Shader; }
	const Texture& GetDiffuseTexture() const noexcept { return *m_DiffuseTex; }
	const Texture& GetSpecularTexture() const noexcept { return *m_SpecularTex; }
	const Texture& GetNormalMap() const noexcept { return *m_NormalMap; }
	float GetSpecularExponent() const noexcept { return m_SpecularExp; }
	glm::vec3 GetDiffuse() const noexcept { return m_Diffuse; }
	glm::vec3 GetSpecular() const noexcept { return m_Specular; }
	glm::vec3 GetAmbient() const noexcept { return m_Ambient; }
	const std::string& GetName() const noexcept { return m_Name; }

private:
	std::shared_ptr<Shader> m_Shader;

	std::shared_ptr<Texture> m_DiffuseTex;
	std::shared_ptr<Texture> m_SpecularTex;
	std::shared_ptr<Texture> m_NormalMap;

	float m_SpecularExp;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	glm::vec3 m_Ambient;
	std::string m_Name;
};
