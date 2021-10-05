#include "Material.h"

void Material::Bind() const noexcept
{
	m_Shader->Bind();
	m_DiffuseTex->Bind(0);
	m_SpecularTex->Bind(1);
	m_NormalMap->Bind(2);
}

void Material::Unbind() const noexcept
{
	m_Shader->Unbind();
	m_DiffuseTex->Unbind();
	m_SpecularTex->Unbind();
	m_NormalMap->Unbind();
}

void Material::UpdateUniforms() const noexcept
{
	m_Shader->SetInt("u_Material.DiffuseTex", 0);
	m_Shader->SetInt("u_Material.SpecularTex", 1);
	m_Shader->SetInt("u_Material.NormalMap", 2);
	m_Shader->SetFloat("u_Material.SpecularExp", m_SpecularExp);
	m_Shader->SetFloat3("u_Material.Diffuse", m_Diffuse);
	m_Shader->SetFloat3("u_Material.Specular", m_Specular);
	m_Shader->SetFloat3("u_Material.Ambient", m_Ambient);
}