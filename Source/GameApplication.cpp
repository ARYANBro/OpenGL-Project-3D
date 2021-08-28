#include "GameApplication.h"

#include "Buffer.h"
#include "Window.h"
#include "RenderingContext.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

void GameApplication::OnBegin()
{
	glfwSetInputMode(GetWindow().GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	auto vertexBuffer = std::make_unique<Buffer>(m_Vertices, sizeof(m_Vertices));

	m_VertexArray.BindVertexBuffer(0, std::move(vertexBuffer), 4 * sizeof(float));
	m_VertexArray.BindAttribute(0, 0);

	AttributeFormat attribFormat = {
		.Size = 4,
		.Type = GL_FLOAT,
		.Normalized = false,
		.RelativeOffset = 0
	};

	m_VertexArray.SetAttributeFormat(0, attribFormat);

	auto indexBuffer = std::make_unique<Buffer>(m_Indices, sizeof(m_Indices));
	m_VertexArray.BindIndexBuffer(std::move(indexBuffer), std::size(m_Indices));

	const char* vertexShaderSrc = R"(
		#version 460

		layout(location = 0) in vec4 a_VertexPos;

		uniform mat4 u_Projection;
		uniform mat4 u_View;

		void main()
		{
			gl_Position = u_Projection * u_View * a_VertexPos;
		}
	)";

	const char* fragmentShaderSrc = R"(
		#version 460

		out vec4 v_Color;

		void main()
		{
			v_Color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
		}
	)";

	try
	{
		m_ShaderProgram.AttachShader(vertexShaderSrc, GL_VERTEX_SHADER);
		m_ShaderProgram.AttachShader(fragmentShaderSrc, GL_FRAGMENT_SHADER);
		m_ShaderProgram.Link();
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

	glfwSwapInterval(1);

	m_VertexArray.EnableAttribute(0);

	m_VertexArray.Bind();
	m_ShaderProgram.Bind();
	glEnable(GL_DEPTH);

	Camera::CameraProperties projectionTransInfo = {
		.Fov = 45.0f,
		.AspectRatio = static_cast<float>(Application::GetInstance().GetWindow().GetProperties().Width) / Application::GetInstance().GetWindow().GetProperties().Height,
		.zNear = 0.01f,
		.zFar = 1000.0f
	};

	m_Camera.SetCameraProperties(projectionTransInfo);
	m_Camera.Translate(glm::vec3{ 0.0f, 0.0f, 3.0f });
	m_Camera.LookAt(glm::vec3{ 0.0f, 0.0f, 0.0f });
	m_Camera.SetMouseSenstivity(0.15f);
	m_Camera.SetSpeed(1.5f);

	m_Camera.OnBegin();
}

void GameApplication::OnUpdate(float deltaTime)
{
	m_Camera.OnUpdate(deltaTime);

	m_ShaderProgram.SetMat4("u_Projection", m_Camera.GetProjectionTransform());
	m_ShaderProgram.SetMat4("u_View", m_Camera.GetViewTransform());
}

void GameApplication::OnRender()
{
	glDrawElements(GL_TRIANGLES, std::size(m_Indices), GL_UNSIGNED_INT, nullptr);
}

void GameApplication::OnEnd()
{
}
