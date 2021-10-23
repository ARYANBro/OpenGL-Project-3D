#include "Renderer.h"

#include "Camera.h"
#include "FileParser.h"
#include "RenderObject.h"

#include <iostream>

Camera* Renderer::s_Camera = nullptr;
std::uint_fast32_t Renderer::s_ShaderID = -1;

void Renderer::Init(Camera* camera) noexcept
{
	s_Camera = camera;

	try
	{
		auto shader = ShaderLibrary::FindIf([](Shader& shader) { return shader.GetName() == "Default Shader"; });

		if (shader == nullptr)
		{
			shader = ShaderLibrary::Find(ShaderLibrary::Create("Default Shader"));

			shader->AttachShader(FileParser::ParseToString("Assets\\Shaders\\CubeVertexShader.glsl"), GL_VERTEX_SHADER);
			shader->AttachShader(FileParser::ParseToString("Assets\\Shaders\\CubeFragmentShader.glsl"), GL_FRAGMENT_SHADER);
			shader->Link();
		}

		s_ShaderID = shader->GetRendererID();
	}
	catch (const ShaderCompilationError& error)
	{
		switch (error.GetShaderType())
		{
			case GL_FRAGMENT_SHADER:
				std::cout << "Fragment ";
				break;

			case GL_VERTEX_SHADER:
				std::cout << "Vertex ";
				break;

			default:
				std::cout << "??? ";
		}

		std::cout << error.what() << std::endl << error.GetInfoLog() << std::endl;
	}
	catch (const ShaderLinkError& error)
	{
		std::cout << error.what() << std::endl << error.GetInfoLog() << std::endl;
	}
}

void Renderer::SetClearColor(glm::vec4 color) noexcept
{
	glClearColor(color.x, color.y, color.z, color.a);
}

void Renderer::OnRender(const std::vector<std::shared_ptr<PointLight>>& pointLights, const std::vector<std::shared_ptr<SpotLight>>& spotLights, const std::vector<std::shared_ptr<DirectionalLight>>& dirLights) noexcept
{
	Shader* shader = ShaderLibrary::Find(s_ShaderID);

	shader->SetMat4("u_View", s_Camera->GetViewTransform());
	shader->SetMat4("u_Projection", s_Camera->GetProjectionTransform());
	shader->SetFloat3("u_CameraPos", s_Camera->GetPosition());

	for (std::size_t i = 0; i < pointLights.size(); i++)
	{
		std::string indexStr = std::to_string(i);

		shader->SetFloat3("u_PointLights[" + indexStr + "].Position", pointLights[i]->GetPosition());
		shader->SetFloat3("u_PointLights[" + indexStr + "].Color.Diffuse", pointLights[i]->GetColor().Diffuse);
		shader->SetFloat3("u_PointLights[" + indexStr + "].Color.Specular", pointLights[i]->GetColor().Specular);
		shader->SetFloat3("u_PointLights[" + indexStr + "].Color.Ambient", pointLights[i]->GetColor().Ambient);
		shader->SetFloat("u_PointLights[" + indexStr + "].Attenuation.Constant", pointLights[i]->GetAttenuation().Constant);
		shader->SetFloat("u_PointLights[" + indexStr + "].Attenuation.Linear", pointLights[i]->GetAttenuation().Linear);
		shader->SetFloat("u_PointLights[" + indexStr + "].Attenuation.Quadratic", pointLights[i]->GetAttenuation().Quadratic);
	}

	for (std::size_t i = 0; i < spotLights.size(); i++)
	{
		std::string indexStr = std::to_string(i);

		shader->SetFloat("u_SpotLights[" + indexStr +"].Attenuation.Constant", spotLights[i]->GetAttenuation().Constant);
		shader->SetFloat("u_SpotLights[" + indexStr +"].Attenuation.Linear", spotLights[i]->GetAttenuation().Linear);
		shader->SetFloat("u_SpotLights[" + indexStr +"].Attenuation.Quadratic", spotLights[i]->GetAttenuation().Quadratic);
		shader->SetFloat3("u_SpotLights[" + indexStr + "].Color.Diffuse", spotLights[i]->GetColor().Diffuse);
		shader->SetFloat3("u_SpotLights[" + indexStr + "].Color.Specular", spotLights[i]->GetColor().Specular);
		shader->SetFloat3("u_SpotLights[" + indexStr + "].Color.Ambient", spotLights[i]->GetColor().Ambient);
		shader->SetFloat3("u_SpotLights[" + indexStr + "].Position", spotLights[i]->GetPosition());
		shader->SetFloat3("u_SpotLights[" + indexStr + "].Direction", spotLights[i]->GetDirection());
		shader->SetFloat("u_SpotLights[" + indexStr + "].CutOff", spotLights[i]->GetCutOff());
		shader->SetFloat("u_SpotLights[" + indexStr + "].OuterCutOff", spotLights[i]->GetOuterCutOff());
	}

	for (std::size_t i = 0; i < dirLights.size(); i++)
	{
		std::string indexStr = std::to_string(i);

		shader->SetFloat3("u_DirLights[" + indexStr + "].Direction", dirLights[i]->GetDirection());
		shader->SetFloat3("u_DirLights[" + indexStr + "].Color.Diffuse", dirLights[i]->GetColor().Diffuse);
		shader->SetFloat3("u_DirLights[" + indexStr + "].Color.Specular", dirLights[i]->GetColor().Specular);
		shader->SetFloat3("u_DirLights[" + indexStr + "].Color.Ambient", dirLights[i]->GetColor().Ambient);
	}

	shader->SetUint("u_NumActivePointLights", pointLights.size());
	shader->SetUint("u_NumActiveSpotLights", spotLights.size());
	shader->SetUint("u_NumActiveDirLights", dirLights.size());

	shader->Bind();
}

void Renderer::Render(const RenderObject& object, const glm::mat4& transform) noexcept
{
	Shader* shader = ShaderLibrary::Find(s_ShaderID);

	for (auto& model : object.GetModels())
	{
		model.GetMaterial().Bind();
		model.GetMaterial().UpdateUniforms();
		model.GetMesh().GetVertexArray().Bind();

		glm::mat4 t = transform * model.GetTransform();

		shader->SetMat4("u_Model", t);
		shader->SetMat4("u_Normal", glm::transpose(glm::inverse(t)));
		glDrawElements(GL_TRIANGLES, model.GetMesh().GetNumVertices(), GL_UNSIGNED_INT, nullptr);

		model.GetMaterial().Unbind();
		model.GetMesh().GetVertexArray().Unbind();
	}
}