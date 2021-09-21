#include "GameApplication.h"

#include "Buffer.h"
#include "Window.h"
#include "RenderingContext.h"
#include "FileParser.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <iostream>
#include <vector>

static void SetShaderVarsPointLight(ShaderProgram& shader, const PointLight& light, std::size_t index) noexcept
{
	std::string indexStr = std::to_string(index);

	shader.SetFloat3("u_PointLights[" + indexStr + "].Position", light.GetPosition());
	shader.SetFloat3("u_PointLights[" + indexStr + "].Color.Diffuse", light.GetColor().Diffuse);
	shader.SetFloat3("u_PointLights[" + indexStr + "].Color.Specular", light.GetColor().Specular);
	shader.SetFloat3("u_PointLights[" + indexStr + "].Color.Ambient", light.GetColor().Ambient);
	shader.SetFloat("u_PointLights[" + indexStr + "].Attenuation.Constant", light.GetAttenuation().Constant);
	shader.SetFloat("u_PointLights[" + indexStr + "].Attenuation.Linear", light.GetAttenuation().Linear);
	shader.SetFloat("u_PointLights[" + indexStr + "].Attenuation.Quadratic", light.GetAttenuation().Quadratic);
}

static void SetShaderVarsDirLight(ShaderProgram& shader, const DirectionalLight& light, std::size_t index)
{
	std::string indexStr = std::to_string(index);

	shader.SetFloat3("u_DirLights[" + indexStr + "].Direction", light.GetDirection());
	shader.SetFloat3("u_DirLights[" + indexStr + "].Color.Diffuse", light.GetColor().Diffuse);
	shader.SetFloat3("u_DirLights[" + indexStr + "].Color.Specular", light.GetColor().Specular);
	shader.SetFloat3("u_DirLights[" + indexStr + "].Color.Ambient", light.GetColor().Ambient);
}

static void SetShaderVarsSpotLight(ShaderProgram& shader, const SpotLight& light, std::size_t index)
{
	std::string indexStr = std::to_string(index);

	shader.SetFloat("u_SpotLights[" + indexStr +"].Attenuation.Constant", light.GetAttenuation().Constant);
	shader.SetFloat("u_SpotLights[" + indexStr +"].Attenuation.Linear", light.GetAttenuation().Linear);
	shader.SetFloat("u_SpotLights[" + indexStr +"].Attenuation.Quadratic", light.GetAttenuation().Quadratic);
	shader.SetFloat3("u_SpotLights[" + indexStr + "].Color.Diffuse", light.GetColor().Diffuse);
	shader.SetFloat3("u_SpotLights[" + indexStr + "].Color.Specular", light.GetColor().Specular);
	shader.SetFloat3("u_SpotLights[" + indexStr + "].Color.Ambient", light.GetColor().Ambient);
	shader.SetFloat3("u_SpotLights[" + indexStr + "].Position", light.GetPosition());
	shader.SetFloat3("u_SpotLights[" + indexStr + "].Direction", light.GetDirection());
	shader.SetFloat("u_SpotLights[" + indexStr + "].CutOff", light.GetCutOff());
	shader.SetFloat("u_SpotLights[" + indexStr + "].OuterCutOff", light.GetOuterCutOff());
}

void GameApplication::OnBegin()
{
	glfwSetInputMode(GetWindow().GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	constexpr glm::vec3 vertices[] = {
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },

		{ -0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f },	

		{ -0.5f,  0.5f, -0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f,  0.5f },

		{ 0.5f,  0.5f, -0.5f },
		{ 0.5f,  0.5f,  0.5f },
		{ 0.5f, -0.5f, -0.5f },
		{ 0.5f, -0.5f,  0.5f },

		{ -0.5f, 0.5f,  0.5f },
		{  0.5f, 0.5f,  0.5f },
		{ -0.5f, 0.5f, -0.5f },
		{  0.5f, 0.5f, -0.5f },

		{ -0.5f, -0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
	};

	constexpr glm::vec2 textureCoords[] = {
		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },

		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },

		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },

		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },

		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },

		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
	};

	constexpr glm::vec3 normals[] = {
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },

		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },

		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },

		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },

		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },

		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f }
	};

	const unsigned int indices[] = {
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

	AttributeFormat attribFormat3 = {
		.Size = 3,
		.Type = GL_FLOAT,
		.Normalized = false,
		.RelativeOffset = 0
	};

	std::vector<glm::vec3> tangents;

	for (int i = 0; i < std::size(indices); i += 3)
	{
		glm::vec3 vertexPos0 = vertices[indices[i]];
		glm::vec3 vertexPos1 = vertices[indices[i + 1]];
		glm::vec3 vertexPos2 = vertices[indices[i + 2]];

		glm::vec2 uv0 = textureCoords[indices[i]];
		glm::vec2 uv1 = textureCoords[indices[i + 1]];
		glm::vec2 uv2 = textureCoords[indices[i + 2]];

		glm::vec3 edge1 = vertexPos1 - vertexPos0;
		glm::vec3 edge2 = vertexPos2 - vertexPos0;
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);

		tangents.push_back(tangent);
		tangents.push_back(tangent);
	}

	auto vertexBuffer = std::make_unique<Buffer>(vertices, sizeof(vertices));
	auto textureCoordsVB = std::make_unique<Buffer>(textureCoords, sizeof(textureCoords));
	auto normalsVB = std::make_unique<Buffer>(normals, sizeof(normals));
	auto tangentVB = std::make_unique<Buffer>(tangents.data(), tangents.size() * sizeof(glm::vec3));
	auto indexBuffer = std::make_unique<Buffer>(indices, sizeof(indices));

	m_VertexArray.BindVertexBuffer(0, std::move(vertexBuffer), 3 * sizeof(float));
	m_VertexArray.BindVertexBuffer(1, std::move(textureCoordsVB), 2 * sizeof(float));
	m_VertexArray.BindVertexBuffer(2, std::move(normalsVB), 3 * sizeof(float));
	m_VertexArray.BindVertexBuffer(3, std::move(tangentVB), 3 * sizeof(float));
	m_VertexArray.BindIndexBuffer(std::move(indexBuffer), std::size(indices));

	m_VertexArray.BindAttribute(0, 0);
	m_VertexArray.BindAttribute(1, 1);
	m_VertexArray.BindAttribute(2, 2);
	m_VertexArray.BindAttribute(3, 3);

	m_VertexArray.SetAttributeFormat(0, attribFormat0);
	m_VertexArray.SetAttributeFormat(1, attribFormat1);
	m_VertexArray.SetAttributeFormat(2, attribFormat2);
	m_VertexArray.SetAttributeFormat(3, attribFormat3);

	try
	{
		m_ShaderProgram.AttachShader(FileParser::ParseToString("Assets\\Shaders\\CubeVertexShader.glsl"), GL_VERTEX_SHADER);
		m_ShaderProgram.AttachShader(FileParser::ParseToString("Assets\\Shaders\\CubeFragmentShader.glsl"), GL_FRAGMENT_SHADER);
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
	m_VertexArray.EnableAttribute(3);

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
		m_DiffuseTex.Load("C:\\Users\\nalin\\Downloads\\cobblestone_floor_08_1k.gltf\\textures\\cobblestone_floor_08_diff_1k.jpg");
		m_SpecularTex.Load("C:\\Users\\nalin\\Downloads\\cobblestone_floor_08_1k.gltf\\textures\\cobblestone_floor_08_rough_1k.jpg");
		m_NormalMap.Load("C:\\Users\\nalin\\Downloads\\cobblestone_floor_08_1k.gltf\\textures\\cobblestone_floor_08_nor_gl_1k.jpg");
	}
	catch (const TextureLoadError& e)
	{
		std::cout << e.what() << std::endl << e.GetFileName() << std::endl;
	}

	LightColor color = {
		.Diffuse = glm::vec3{ 1.0f },
		.Specular = glm::vec3{ 1.0f },
		.Ambient = glm::vec3{ 0.2f }
	};

	AttenuationTerms attenuation = {
		.Constant = 1.0f,
		.Linear = 0.9f,
		.Quadratic = 0.032f
	};

	m_PointLight.SetColor(color);
	m_PointLight.SetAttenuation(attenuation);
	m_PointLight.SetPosition(glm::vec3{ 1.0f, 1.0f, 1.0f });

	m_DirLight.SetColor(LightColor{ 
		.Diffuse = glm::vec3(0.0f),
		.Specular = glm::vec3(0.0f),
		.Ambient = glm::vec3(0.0f)
	});
	
	m_DirLight.SetDirection(glm::vec3{ -0.75f, -1.0f, 5.0f});

	m_SpotLight.SetAttenuation(attenuation);

	m_SpotLight.SetColor(LightColor{
		.Diffuse = glm::vec3{ 0.75f },
		.Specular = glm::vec3{ 0.75f },
		.Ambient = glm::vec3{ 0.1f }
	});
	
	m_SpotLight.SetCutOff(std::cos(glm::radians(12.5f)));
	m_SpotLight.SetOuterCutOff(std::cos(glm::radians(16.5f)));

}

void GameApplication::OnUpdate(float deltaTime)
{
	m_Camera.OnUpdate(deltaTime);
}

void GameApplication::OnRender()
{
	glClearColor(0.07f, 0.07f, 0.08f, 1.0f);

	m_DiffuseTex.Bind(0);
	m_SpecularTex.Bind(1);
	m_NormalMap.Bind(2);

	m_SpotLight.SetDirection(m_Camera.GetDirectionalVectors().Forward);
	m_SpotLight.SetPosition(m_Camera.GetPosition());

	SetShaderVarsSpotLight(m_ShaderProgram, m_SpotLight, 0);
	SetShaderVarsPointLight(m_ShaderProgram, m_PointLight, 0);
	SetShaderVarsDirLight(m_ShaderProgram, m_DirLight, 0);

	glm::mat4 modelTransform = glm::mat4(1.0f);
	m_ShaderProgram.SetMat4("u_Model", modelTransform);
	m_ShaderProgram.SetMat4("u_View", m_Camera.GetViewTransform());
	m_ShaderProgram.SetMat4("u_Projection", m_Camera.GetProjectionTransform());
	m_ShaderProgram.SetMat4("u_Normal", glm::transpose(glm::inverse(modelTransform)));
	m_ShaderProgram.SetInt("u_DiffuseTex", 0);
	m_ShaderProgram.SetInt("u_SpecularTex", 1);
	m_ShaderProgram.SetInt("u_NormalMap", 2);
	m_ShaderProgram.SetFloat("u_Smoothness", 0.15f);
	m_ShaderProgram.SetFloat3("u_Color", glm::vec3{ 0.0f});
	m_ShaderProgram.SetFloat3("u_CameraPos", m_Camera.GetPosition());
	m_ShaderProgram.SetUint("u_NumActivePointLights", 1);
	m_ShaderProgram.SetUint("u_NumActiveSpotLights", 1);
	m_ShaderProgram.SetUint("u_NumActiveDirLights", 1);

	glDrawElements(GL_TRIANGLES, m_NumVertexIndex, GL_UNSIGNED_INT, nullptr);

	m_DiffuseTex.Unbind();
	m_SpecularTex.Unbind();
	m_NormalMap.Unbind();

	m_ShaderProgram.SetMat4("u_Model", glm::scale(glm::translate(glm::mat4(1.0f), m_PointLight.GetPosition()), glm::vec3(0.2f)));
	m_ShaderProgram.SetFloat3("u_Color", glm::vec3{ 1.0f});

	glDrawElements(GL_TRIANGLES, m_NumVertexIndex, GL_UNSIGNED_INT, nullptr);
}

void GameApplication::OnEnd()
{
}
