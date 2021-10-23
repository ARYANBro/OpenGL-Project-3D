#include "Material.h"

void Material::Bind() const noexcept
{
	if (m_ShaderID != 0)
		ShaderLibrary::Find(m_ShaderID)->Bind();

	if (m_DiffuseTexID != 0)
		TextureLibrary::Find(m_DiffuseTexID)->Bind(0);

	if (m_SpecularTexID != 0)
		TextureLibrary::Find(m_SpecularTexID)->Bind(1);

	if (m_NormalMapID != 0)
		TextureLibrary::Find(m_NormalMapID)->Bind(2);
}

void Material::Unbind() const noexcept
{
	if (m_ShaderID != 0)
		ShaderLibrary::Find(m_ShaderID)->Unbind();

	if (m_DiffuseTexID != 0)
		TextureLibrary::Find(m_DiffuseTexID)->Unbind();

	if (m_SpecularTexID != 0)
		TextureLibrary::Find(m_SpecularTexID)->Unbind();

	if (m_NormalMapID != 0)
		TextureLibrary::Find(m_NormalMapID)->Unbind();
}

void Material::UpdateUniforms() const noexcept
{
	if (m_ShaderID != 0)
	{
		auto shader = ShaderLibrary::Find(m_ShaderID);

		if (m_DiffuseTexID != 0)
			shader->SetInt("u_Material.DiffuseTex", 0);

		if (m_SpecularTexID != 0)
			shader->SetInt("u_Material.SpecularTex", 1);

		if (m_NormalMapID != 0)
			shader->SetInt("u_Material.NormalMap", 2);
			
		shader->SetFloat("u_Material.SpecularExp", m_SpecularExp);
		shader->SetFloat3("u_Material.Diffuse", m_Diffuse);
		shader->SetFloat3("u_Material.Specular", m_Specular);
		shader->SetFloat3("u_Material.Ambient", m_Ambient);
	}
}
