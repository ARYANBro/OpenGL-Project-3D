#include "GameApplication.h"

#include "Buffer.h"
#include "Window.h"
#include "RenderingContext.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <iostream>

void GameApplication::OnBegin()
{
	glfwSetInputMode(GetWindow().GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	AttributeFormat attribFormat0 = {
		.Size = 3,
		.Type = GL_FLOAT,
		.Normalized = false,
		.RelativeOffset = 0
	};

	AttributeFormat attribFormat1 = {
		.Size = 2,
		.Type = GL_FLOAT,
		.Normalized = false,
		.RelativeOffset = 0
	};

	AttributeFormat attribFormat2 = {
		.Size = 3,
		.Type = GL_FLOAT,
		.Normalized = false,
		.RelativeOffset = 0
	};
	
	auto vertexBuffer = std::make_unique<Buffer>(m_Vertices, sizeof(m_Vertices));
	auto textureCoordsVB = std::make_unique<Buffer>(m_TextureCoords, sizeof(m_TextureCoords));
	auto normalsVB = std::make_unique<Buffer>(m_Normals, sizeof(m_Normals));
	auto indexBuffer = std::make_unique<Buffer>(m_Indices, sizeof(m_Indices));

	m_VertexArray.BindVertexBuffer(0, std::move(vertexBuffer), 3 * sizeof(float));
	m_VertexArray.BindVertexBuffer(1, std::move(textureCoordsVB), 2 * sizeof(float));
	m_VertexArray.BindVertexBuffer(2, std::move(normalsVB), 3 * sizeof(float));
	m_VertexArray.BindIndexBuffer(std::move(indexBuffer), std::size(m_Indices));

	m_VertexArray.BindAttribute(0, 0);
	m_VertexArray.BindAttribute(1, 1);
	m_VertexArray.BindAttribute(2, 2);

	m_VertexArray.SetAttributeFormat(0, attribFormat0);
	m_VertexArray.SetAttributeFormat(1, attribFormat1);
	m_VertexArray.SetAttributeFormat(2, attribFormat2);

	const char* vertexShaderSrc = R"(
		#version 460

		layout(location = 0) in vec4 a_VertexPos;
		layout(location = 1) in vec2 a_TextureCoord;
		layout(location = 2) in vec4 a_Normal;

		out vec2 v_TextureCoord;
		out vec3 v_Normal;
		out vec3 v_FragmentPos;

		uniform mat4 u_Projection;
		uniform mat4 u_View;
		uniform mat4 u_Model;
		uniform mat4 u_Normal;

		void main()
		{
			v_TextureCoord = a_TextureCoord;
			v_Normal = vec3(u_Normal * a_Normal);

			v_FragmentPos = vec3(u_Model * a_VertexPos);
			gl_Position = u_Projection * u_View * u_Model * a_VertexPos;
		}
	)";

	const char* fragmentShaderSrc = R"(
		#version 460

		out vec4 v_Color;

		in vec2 v_TextureCoord;
		in vec3 v_Normal;
		in vec3 v_FragmentPos;

		uniform sampler2D u_DiffuseTex;
		uniform sampler2D u_SpecularTex;

		uniform vec3 u_Color;
		uniform float u_Ambient;
		uniform float u_Smoothness;
		// uniform float u_SpecularIntensity;

		uniform vec3 u_LightPos;
		uniform vec3 u_LightColor;

		uniform vec3 u_ViewPos;

		void main()
		{
			// Diffuse
			vec3 normal = normalize(v_Normal);
			vec3 lightDir = normalize(u_LightPos - v_FragmentPos);
			float lightDotNorm = clamp(dot(normal, lightDir), 0.0f, 1.0f);

			// Reflection
			vec3 viewDir = normalize(u_ViewPos - v_FragmentPos);

			vec3 reflectDir = normalize(reflect(-lightDir, normal));
			float viewDotReflect = clamp(dot(reflectDir, viewDir), 0.0f, 1.0f);
			float specular = pow(viewDotReflect, u_Smoothness) * texture2D(u_SpecularTex, v_TextureCoord).x;

			vec4 diffuseCol = texture2D(u_DiffuseTex, v_TextureCoord);

			vec4 diffuse = diffuseCol * lightDotNorm;
			vec4 ambient = diffuseCol * u_Ambient;

			v_Color = (diffuse + ambient + specular) * vec4(u_Color, 0.0f) * vec4(u_LightColor, 0.0f);
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
	m_VertexArray.EnableAttribute(1);
	m_VertexArray.EnableAttribute(2);

	m_VertexArray.Bind();
	m_ShaderProgram.Bind();

	glEnable(GL_DEPTH_TEST);

	Camera::CameraProps cameraProps = {
		.Fov = 45.0f,
		.AspectRatio = static_cast<float>(Application::GetInstance().GetWindow().GetProperties().Width) / Application::GetInstance().GetWindow().GetProperties().Height,
		.zNear = 0.01f,
		.zFar = 1000.0f
	};

	m_Camera.SetCameraProperties(cameraProps);
	m_Camera.Translate(glm::vec3{ 0.0f, 0.0f, 3.0f });
	m_Camera.LookAt(glm::vec3{ 0.0f, 0.0f, 0.0f });
	m_Camera.SetMouseSenstivity(0.15f);
	m_Camera.SetSpeed(1.5f);

	m_Camera.OnBegin();

	try
	{
		m_SpecularTex.Load("C:\\Users\\nalin\\Downloads\\PavingStones064_1K-PNG\\PavingStones067_1K_Roughness.png");
		m_DiffuseTex.Load("C:\\Users\\nalin\\Downloads\\PavingStones064_1K-PNG\\PavingStones067_1K_Color.png");
	}
	catch (const TextureLoadError& e)
	{
		std::cout << e.what() << std::endl << e.GetFileName() << std::endl;
	}

	m_DiffuseTex.Bind(0);
	m_SpecularTex.Bind(1);

	m_ShaderProgram.SetInt("u_DiffuseTex", 0);
	m_ShaderProgram.SetInt("u_SpecularTex", 1);

	m_ShaderProgram.SetFloat("u_Ambient", 0.2f);
	m_ShaderProgram.SetFloat("u_Smoothness", 32);
	m_ShaderProgram.SetFloat3("u_Color", glm::vec3{ 1.0f, 1.0f, 1.0f });

	m_ShaderProgram.SetFloat3("u_LightPos", glm::vec3{ 1.0f, 1.0f, 1.0f });
	m_ShaderProgram.SetFloat3("u_LightColor", glm::vec3{ 1.0f, 1.0f, 1.0f });
}

void GameApplication::OnUpdate(float deltaTime)
{
	m_Camera.OnUpdate(deltaTime);

	glm::mat4 modelTransform = glm::mat4(1.0f);

	m_ShaderProgram.SetMat4("u_Model", modelTransform);
	m_ShaderProgram.SetMat4("u_View", m_Camera.GetViewTransform());
	m_ShaderProgram.SetMat4("u_Projection", m_Camera.GetProjectionTransform());
	m_ShaderProgram.SetMat4("u_Normal", glm::transpose(glm::inverse(modelTransform)));

	m_ShaderProgram.SetFloat3("u_ViewPos", m_Camera.GetPosition());
}

void GameApplication::OnRender()
{
	glClearColor(0.07f, 0.07f, 0.08f, 1.0f);
	glDrawElements(GL_TRIANGLES, std::size(m_Indices), GL_UNSIGNED_INT, nullptr);
}

void GameApplication::OnEnd()
{
}
