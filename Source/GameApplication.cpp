#include "GameApplication.h"

#include "FileParser.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <iostream>

void GameApplication::OnBegin()
{
	glfwSetInputMode(GetWindow().GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);

	Renderer::Init(&m_Camera);

	m_Backpack = ModelLoader3D::ReadFile("Assets\\3DModels\\Backpack\\backpack.obj", aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
	m_Plane = ModelLoader3D::ReadFile("Assets\\3DModels\\Plane\\Plane.obj", aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	
	glEnable(GL_DEPTH_TEST);

	Camera::CameraProps cameraProps = {
		.Fov = 45.0f,
		.AspectRatio = static_cast<float>(Application::GetInstance().GetWindow().GetProperties().Width) / Application::GetInstance().GetWindow().GetProperties().Height,
		.zNear = 0.01f,
		.zFar = 1000.0f
	};

	m_Camera.SetCameraProperties(cameraProps);
	m_Camera.Translate(glm::vec3{ 0.0f, 2.0f, 2.0f });
	m_Camera.LookAt(glm::vec3{ 0.0f, 0.0f, 0.0f });
	m_Camera.SetMouseSenstivity(0.15f);
	m_Camera.SetSpeed(1.5f);
	m_Camera.OnBegin();

	m_PointLight = m_PointLights.emplace_back(std::make_shared<PointLight>());
	m_SpotLight = m_SpotLights.emplace_back(std::make_shared<SpotLight>());
	m_DirLight = m_DirLights.emplace_back(std::make_shared<DirectionalLight>());

	m_PointLight->SetColor(LightColor{
		.Diffuse = glm::vec3{ 1.0f },
		.Specular = glm::vec3{ 1.0f },
		.Ambient = glm::vec3{ 0.2f }
	});

	m_PointLight->SetAttenuation(AttenuationTerms{
		.Constant = 1.0f,
		.Linear = 0.9f,
		.Quadratic = 0.032f
	});

	m_PointLight->SetPosition(glm::vec3{ 1.0f, 1.0f, 1.0f });

	m_DirLight->SetColor(LightColor{ 
		.Diffuse = glm::vec3(0.0f),
		.Specular = glm::vec3(0.0f),
		.Ambient = glm::vec3(0.0f)
	});
	
	m_DirLight->SetDirection(glm::vec3{ -0.75f, -1.0f, 5.0f});

	m_SpotLight->SetAttenuation(m_PointLight->GetAttenuation());
	m_SpotLight->SetColor(LightColor{
		.Diffuse = glm::vec3{ 0.75f },
		.Specular = glm::vec3{ 0.75f },
		.Ambient = glm::vec3{ 0.1f }
	});
	
	m_SpotLight->SetCutOff(std::cos(glm::radians(12.5f)));
	m_SpotLight->SetOuterCutOff(std::cos(glm::radians(16.5f)));

	glm::mat4 translated = glm::translate(glm::mat4(1.0f), glm::vec3{ 2.0f, 0.6f, 0.0f });
	m_Backpack.SetTransform(glm::scale(translated, glm::vec3(0.3f)));
}

void GameApplication::OnUpdate(float deltaTime)
{
	m_Camera.OnUpdate(deltaTime);
	m_SpotLight->SetDirection(m_Camera.GetDirectionalVectors().Forward);
	m_SpotLight->SetPosition(m_Camera.GetPosition());
}

void GameApplication::OnRender()
{
	Renderer::SetClearColor({ 0.07f, 0.07f, 0.08f, 1.0f });
	Renderer::OnRender(m_PointLights, m_SpotLights, m_DirLights);
	Renderer::Render(m_Backpack, m_Backpack.GetTransform());
	Renderer::Render(m_Plane, m_Plane.GetTransform());
}

void GameApplication::OnEnd()
{
}
