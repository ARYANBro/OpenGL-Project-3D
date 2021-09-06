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

	const float vertices[72] = {
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, 0.5f,
		 0.5f,  0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		 0.5f, -0.5f, 0.5f,

		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,

		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,

		 0.5f, 0.5f, -0.5f,
		 0.5f, 0.5f,  0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f,  0.5f,

		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f
	};

	const float textureCoords[48] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	const float normals[72] = {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f
	};

	const unsigned int indices[36] = {
		0, 1, 2,
		2, 1, 3,

		4, 5, 6,
		6, 5, 7,

		8, 9, 10,
		10, 9, 11,

		12, 13, 14,
		14, 13, 15,

		16, 17, 18,
		18, 17, 19,

		20, 21, 22,
		22, 21, 23
	};

	m_NumVertexIndex = std::size(indices);

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
	
	auto vertexBuffer = std::make_unique<Buffer>(vertices, sizeof(vertices));
	auto textureCoordsVB = std::make_unique<Buffer>(textureCoords, sizeof(textureCoords));
	auto normalsVB = std::make_unique<Buffer>(normals, sizeof(normals));
	auto indexBuffer = std::make_unique<Buffer>(indices, sizeof(indices));

	m_VertexArray.BindVertexBuffer(0, std::move(vertexBuffer), 3 * sizeof(float));
	m_VertexArray.BindVertexBuffer(1, std::move(textureCoordsVB), 2 * sizeof(float));
	m_VertexArray.BindVertexBuffer(2, std::move(normalsVB), 3 * sizeof(float));
	m_VertexArray.BindIndexBuffer(std::move(indexBuffer), std::size(indices));

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

		struct Material
		{
			sampler2D DiffuseTex;
			sampler2D SpecularTex;
			vec3 Color;
			float AmbientAmt;
			float SpecularAmt;
			float Smoothness;
		};

		struct LightColor
		{
			vec3 Color;
			vec3 SpecularColor;
			vec3 AmbientColor;
		};

		struct AttenuationTerms
		{
			float Constant;
			float Linear;
			float Quadratic;
		};

		struct PointLight
		{
			vec3 Position;
			LightColor Color;
			AttenuationTerms Attenuation;
		};

		uniform Material u_Material;
		uniform PointLight u_PointLight;

		uniform vec3 u_ViewPos;

		float CalculateAttenuation(vec3 fragmentPos, vec3 lightPos, AttenuationTerms attenuation)
		{
			float distance = length(lightPos - fragmentPos);

			float constant = attenuation.Constant;
			float linear = attenuation.Linear * distance;
			float quadratic = attenuation.Quadratic * (distance * distance);

			return 1.0 / (constant + linear + quadratic);
		}

		void main()
		{
			// Diffuse
			vec3 normal = normalize(v_Normal);
			vec3 lightDir = normalize(u_PointLight.Position - v_FragmentPos);
			float diffuseAmt = clamp(dot(normal, lightDir), 0.0f, 1.0f);

			// Reflection
			vec3 viewDir = normalize(u_ViewPos - v_FragmentPos);
			vec3 reflectDir = normalize(reflect(-lightDir, normal));
			float viewDotReflect = clamp(dot(reflectDir, viewDir), 0.0f, 1.0f);
			float specularAmt = pow(viewDotReflect, u_Material.Smoothness * 256);

			vec4 specularTex = texture2D(u_Material.SpecularTex, v_TextureCoord);
			vec4 diffuseCol = texture2D(u_Material.DiffuseTex, v_TextureCoord);

			vec4 specular = specularAmt * specularTex.r * u_Material.SpecularAmt * vec4(u_PointLight.Color.SpecularColor, 1.0f);
			vec4 diffuse = diffuseCol * diffuseAmt * vec4(u_PointLight.Color.Color, 0.0f);
			vec4 ambient = diffuseCol * u_Material.AmbientAmt * vec4(u_PointLight.Color.AmbientColor, 0.0f);

			float attenuation = CalculateAttenuation(v_FragmentPos, u_PointLight.Position, u_PointLight.Attenuation);

			diffuse *= attenuation;
			specular *= attenuation;
			ambient *= attenuation;

			v_Color = (diffuse + ambient + specular) * vec4(u_Material.Color, 0.0f);
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
	m_Camera.Translate(glm::vec3{ 0.0f, 0.0f, 2.0f });
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

	m_ShaderProgram.SetInt("u_Material.DiffuseTex", 0);
	m_ShaderProgram.SetInt("u_Material.SpecularTex", 1);

	m_ShaderProgram.SetFloat("u_Material.AmbientAmt", 0.2f);
	m_ShaderProgram.SetFloat("u_Material.Smoothness", 0.2f);
	m_ShaderProgram.SetFloat3("u_Material.Color", glm::vec3{ 1.0f, 1.0f, 1.0f });
	m_ShaderProgram.SetFloat("u_Material.SpecularAmt", 1.0f);

	LightColor color = {
		.Color = glm::vec3{ 1.0f, 1.0f, 1.0f },
		.SpecularColor = glm::vec3{ 1.0f, 1.0f, 1.0f },
		.AmbientColor = glm::vec3{ 1.0f, 1.0f, 1.0f }
	};

	AttenuationTerms attenuation = {
		.Constant = 1.0,
		.Linear = 0.9f,
		.Quadratic = 0.032f
	};

	m_PointLight.SetColor(color);
	m_PointLight.SetAttenuation(attenuation);

	m_ShaderProgram.SetFloat3("u_PointLight.Color.Color", m_PointLight.GetColor().Color);
	m_ShaderProgram.SetFloat3("u_PointLight.Color.SpecularColor", m_PointLight.GetColor().SpecularColor);
	m_ShaderProgram.SetFloat3("u_PointLight.Color.AmbientColor", m_PointLight.GetColor().AmbientColor);

	m_ShaderProgram.SetFloat("u_PointLight.Attenuation.Constant", m_PointLight.GetAttenuation().Constant);
	m_ShaderProgram.SetFloat("u_PointLight.Attenuation.Linear", m_PointLight.GetAttenuation().Linear);
	m_ShaderProgram.SetFloat("u_PointLight.Attenuation.Quadratic", m_PointLight.GetAttenuation().Quadratic);
}

void GameApplication::OnUpdate(float deltaTime)
{
	m_Camera.OnUpdate(deltaTime);

	glm::mat4 modelTransform = glm::mat4(1.0f);
	m_PointLight.SetPosition(m_Camera.GetPosition());

	m_ShaderProgram.SetFloat3("u_PointLight.Position", m_PointLight.GetPosition());

	m_ShaderProgram.SetMat4("u_Model", modelTransform);
	m_ShaderProgram.SetMat4("u_View", m_Camera.GetViewTransform());
	m_ShaderProgram.SetMat4("u_Projection", m_Camera.GetProjectionTransform());
	m_ShaderProgram.SetMat4("u_Normal", glm::transpose(glm::inverse(modelTransform)));

	m_ShaderProgram.SetFloat3("u_ViewPos", m_Camera.GetPosition());
}

void GameApplication::OnRender()
{
	glClearColor(0.07f, 0.07f, 0.08f, 1.0f);
	glDrawElements(GL_TRIANGLES, m_NumVertexIndex, GL_UNSIGNED_INT, nullptr);
}

void GameApplication::OnEnd()
{
}
