#include "Renderer.h"

#include "Camera.h"
#include "FileParser.h"
#include "RenderObject.h"

#include <iostream>

Camera* Renderer::s_Camera = nullptr;
std::shared_ptr<Shader> Renderer::s_Shader = nullptr;

void Renderer::Init(Camera* camera) noexcept
{
	s_Camera = camera;

	try
	{
		s_Shader = ShaderLibrary::Load("Default Shader");

		s_Shader->AttachShader(FileParser::ParseToString("Assets\\Shaders\\CubeVertexShader.glsl"), GL_VERTEX_SHADER);
		s_Shader->AttachShader(FileParser::ParseToString("Assets\\Shaders\\CubeFragmentShader.glsl"), GL_FRAGMENT_SHADER);
		s_Shader->Link();
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
	s_Shader->SetMat4("u_View", s_Camera->GetViewTransform());
	s_Shader->SetMat4("u_Projection", s_Camera->GetProjectionTransform());
	s_Shader->SetFloat3("u_CameraPos", s_Camera->GetPosition());

	for (std::size_t i = 0; i < pointLights.size(); i++)
	{
		std::string indexStr = std::to_string(i);

		s_Shader->SetFloat3("u_PointLights[" + indexStr + "].Position", pointLights[i]->GetPosition());
		s_Shader->SetFloat3("u_PointLights[" + indexStr + "].Color.Diffuse", pointLights[i]->GetColor().Diffuse);
		s_Shader->SetFloat3("u_PointLights[" + indexStr + "].Color.Specular", pointLights[i]->GetColor().Specular);
		s_Shader->SetFloat3("u_PointLights[" + indexStr + "].Color.Ambient", pointLights[i]->GetColor().Ambient);
		s_Shader->SetFloat("u_PointLights[" + indexStr + "].Attenuation.Constant", pointLights[i]->GetAttenuation().Constant);
		s_Shader->SetFloat("u_PointLights[" + indexStr + "].Attenuation.Linear", pointLights[i]->GetAttenuation().Linear);
		s_Shader->SetFloat("u_PointLights[" + indexStr + "].Attenuation.Quadratic", pointLights[i]->GetAttenuation().Quadratic);
	}

	for (std::size_t i = 0; i < spotLights.size(); i++)
	{
		std::string indexStr = std::to_string(i);

		s_Shader->SetFloat("u_SpotLights[" + indexStr +"].Attenuation.Constant", spotLights[i]->GetAttenuation().Constant);
		s_Shader->SetFloat("u_SpotLights[" + indexStr +"].Attenuation.Linear", spotLights[i]->GetAttenuation().Linear);
		s_Shader->SetFloat("u_SpotLights[" + indexStr +"].Attenuation.Quadratic", spotLights[i]->GetAttenuation().Quadratic);
		s_Shader->SetFloat3("u_SpotLights[" + indexStr + "].Color.Diffuse", spotLights[i]->GetColor().Diffuse);
		s_Shader->SetFloat3("u_SpotLights[" + indexStr + "].Color.Specular", spotLights[i]->GetColor().Specular);
		s_Shader->SetFloat3("u_SpotLights[" + indexStr + "].Color.Ambient", spotLights[i]->GetColor().Ambient);
		s_Shader->SetFloat3("u_SpotLights[" + indexStr + "].Position", spotLights[i]->GetPosition());
		s_Shader->SetFloat3("u_SpotLights[" + indexStr + "].Direction", spotLights[i]->GetDirection());
		s_Shader->SetFloat("u_SpotLights[" + indexStr + "].CutOff", spotLights[i]->GetCutOff());
		s_Shader->SetFloat("u_SpotLights[" + indexStr + "].OuterCutOff", spotLights[i]->GetOuterCutOff());
	}

	for (std::size_t i = 0; i < dirLights.size(); i++)
	{
		std::string indexStr = std::to_string(i);

		s_Shader->SetFloat3("u_DirLights[" + indexStr + "].Direction", dirLights[i]->GetDirection());
		s_Shader->SetFloat3("u_DirLights[" + indexStr + "].Color.Diffuse", dirLights[i]->GetColor().Diffuse);
		s_Shader->SetFloat3("u_DirLights[" + indexStr + "].Color.Specular", dirLights[i]->GetColor().Specular);
		s_Shader->SetFloat3("u_DirLights[" + indexStr + "].Color.Ambient", dirLights[i]->GetColor().Ambient);
	}

	s_Shader->SetUint("u_NumActivePointLights", pointLights.size());
	s_Shader->SetUint("u_NumActiveSpotLights", spotLights.size());
	s_Shader->SetUint("u_NumActiveDirLights", dirLights.size());

	s_Shader->Bind();
}

void Renderer::Render(const RenderObject& object, const glm::mat4& transform) noexcept
{
	for (auto& model : object.GetModels())
	{
		model->GetMaterial().Bind();
		model->GetMaterial().UpdateUniforms();
		model->GetMesh().GetVertexArray().Bind();

		glm::mat4 t = transform * model->GetTransform();

		s_Shader->SetMat4("u_Model", t);
		s_Shader->SetMat4("u_Normal", glm::transpose(glm::inverse(t)));
		glDrawElements(GL_TRIANGLES, model->GetMesh().GetNumVertices(), GL_UNSIGNED_INT, nullptr);
	}
}