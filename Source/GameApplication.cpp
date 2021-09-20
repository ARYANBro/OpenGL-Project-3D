#include "GameApplication.h"

#include "Buffer.h"
#include "Window.h"
#include "RenderingContext.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <iostream>
#include <vector>

void GameApplication::OnBegin()
{
	glfwSetInputMode(GetWindow().GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	constexpr glm::vec3 vertices[] = {
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },

		{ -0.5f,  0.5f, 0.5f },
		{  0.5f,  0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f },
		{  0.5f, -0.5f, 0.5f },	

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

	const char* vertexShaderSrc = R"(
		#version 460

		layout(location = 0) in vec4 a_VertexPos;
		layout(location = 1) in vec2 a_TextureCoord;
		layout(location = 2) in vec4 a_Normal;
		layout(location = 3) in vec4 a_Tangent;

		out vec2 v_TextureCoord;
		out vec3 v_Normal;
		out vec3 v_FragmentPos;
		out mat3 v_TBN;

		uniform mat4 u_Projection;
		uniform mat4 u_View;
		uniform mat4 u_Model;
		uniform mat4 u_Normal;

		void main()
		{
			v_TextureCoord = a_TextureCoord;
			v_FragmentPos = vec3(u_Model * a_VertexPos);

			vec3 bitangent = cross(a_Normal.xyz, a_Tangent.xyz);
			bitangent = normalize(vec3(u_Model * vec4(bitangent, 0.0f)));

			v_Normal = normalize(vec3(u_Normal * a_Normal));

			vec3 tangent = normalize(vec3(u_Model * a_Tangent));
			tangent = normalize(tangent - dot(tangent, v_Normal) * v_Normal);

			v_TBN = mat3(tangent, bitangent, v_Normal);

			gl_Position = u_Projection * u_View * u_Model * a_VertexPos;
		}
	)";

	const char* fragmentShaderSrc = R"(
		#version 460

		out vec4 v_Color;

		in vec2 v_TextureCoord;
		in vec3 v_Normal;
		in vec3 v_FragmentPos;
		in mat3 v_TBN;

		struct LightColor
		{
			vec3 Diffuse;
			vec3 Specular;
			vec3 Ambient;
		};

		struct AttenuationTerms
		{
			float Constant;
			float Linear;
			float Quadratic;
		};

		struct SpotLight
		{
			vec3 Position;
			vec3 Direction;
			LightColor Color;
			AttenuationTerms Attenuation;
			float CutOff;
			float OuterCutOff;
		};

		struct PointLight
		{
			vec3 Position;
			LightColor Color;
			AttenuationTerms Attenuation;
		};

		struct DirectionalLight
		{
			vec3 Direction;
			LightColor Color;
		};

		uniform vec3 u_CameraPos;
		uniform sampler2D u_DiffuseTex;
		uniform sampler2D u_SpecularTex;
		uniform sampler2D u_NormalMap;
		uniform vec3 u_Color;
		uniform float u_Smoothness;
		uniform SpotLight u_SpotLight;
		uniform PointLight u_PointLight;
		uniform DirectionalLight u_DirLight;

		vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 cameraPos)
		{
			vec3 ambient = spotLight.Color.Ambient * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

			vec3 lightDir = normalize(spotLight.Position - v_FragmentPos);
			float diffuseAmt = max(dot(normal, lightDir), 0.0f);
			vec3 diffuse = spotLight.Color.Diffuse * diffuseAmt * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

			vec3 cameraDir = normalize(cameraPos - v_FragmentPos);
			vec3 reflectDir = reflect(-lightDir, normal);
			float specularAmt = pow(max(dot(reflectDir, cameraDir), 0.0f), u_Smoothness * 256);
			vec3 specular = spotLight.Color.Specular * specularAmt * texture2D(u_SpecularTex, v_TextureCoord).r;

			float theta = dot(lightDir, normalize(-spotLight.Direction));
			float epsilon = spotLight.CutOff - spotLight.OuterCutOff;
			float intensity = clamp((theta - spotLight.OuterCutOff) / epsilon, 0.0f, 1.0f);

			float distance = length(spotLight.Position - v_FragmentPos);
			float constant = spotLight.Attenuation.Constant;
			float linear = spotLight.Attenuation.Linear * distance;
			float quadratic = spotLight.Attenuation.Quadratic * distance * distance;
			float attenuation = 1.0f / (constant + linear + quadratic);

			diffuse *= attenuation * intensity;
			specular *= attenuation * intensity;
			ambient *= attenuation;

			return (diffuse + specular + ambient);
		}

		vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 cameraPos)
		{
			vec3 ambient = pointLight.Color.Ambient * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

			vec3 lightDir = normalize(pointLight.Position - v_FragmentPos);
			float diffuseAmt = max(dot(normal, lightDir), 0.0f);
			vec3 diffuse = pointLight.Color.Diffuse * diffuseAmt * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

			vec3 cameraDir = normalize(cameraPos - v_FragmentPos);
			vec3 reflectDir = reflect(-lightDir, normal);
			float specularAmt = pow(max(dot(reflectDir, cameraDir), 0.0f), u_Smoothness * 256);
			vec3 specular = pointLight.Color.Specular * specularAmt * texture2D(u_SpecularTex, v_TextureCoord).r;

			float distance = length(pointLight.Position - v_FragmentPos);
			float constant = pointLight.Attenuation.Constant;
			float linear = pointLight.Attenuation.Linear * distance;
			float quadratic = pointLight.Attenuation.Quadratic * distance * distance;
			float attenuation = 1.0 / (constant + linear + quadratic);

			diffuse *= attenuation;
			specular *= attenuation;
			ambient *= attenuation;

			return (diffuse + specular + ambient);
		}

		vec3 CalculateDirLight(DirectionalLight dirLight, vec3 normal, vec3 cameraPos)
		{
			vec3 ambient = dirLight.Color.Ambient * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

			vec3 lightDir = normalize(-dirLight.Direction);
			float diffuseAmt = max(dot(normal, lightDir), 0.0f);
			vec3 diffuse = dirLight.Color.Diffuse * diffuseAmt * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

			vec3 cameraDir = normalize(cameraPos - v_FragmentPos);
			vec3 reflectDir = reflect(-lightDir, normal);
			float specularAmt = pow(max(dot(reflectDir, cameraDir), 0.0f), u_Smoothness * 256);
			vec3 specular = dirLight.Color.Specular * specularAmt * texture2D(u_SpecularTex, v_TextureCoord).r;

			return (diffuse + specular + ambient);
		}

		void main()
		{
			vec3 normal = (2 * texture2D(u_NormalMap, v_TextureCoord).rgb) - 1;
			normal = normalize(v_TBN * normal);

			vec3 spotLight = CalculateSpotLight(u_SpotLight, normalize(normal), u_CameraPos);
			vec3 pointLight = CalculatePointLight(u_PointLight, normalize(normal), u_CameraPos);
			vec3 dirLight = CalculateDirLight(u_DirLight, normalize(normal), u_CameraPos);

			// vec3 spotLight = CalculateSpotLight(u_SpotLight, normalize(v_Normal), u_CameraPos);
			// vec3 pointLight = CalculatePointLight(u_PointLight, normalize(v_Normal), u_CameraPos);
			// vec3 dirLight = CalculateDirLight(u_DirLight, normalize(v_Normal), u_CameraPos);

			v_Color = vec4(spotLight + pointLight + dirLight + u_Color, 1.0f);
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
	m_DirLight.SetDirection(glm::vec3{ -0.75f, -1.0f, 0.0f});

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

	m_ShaderProgram.SetInt("u_DiffuseTex", 0);
	m_ShaderProgram.SetInt("u_SpecularTex", 1);
	m_ShaderProgram.SetInt("u_NormalMap", 2);
	m_ShaderProgram.SetFloat("u_Smoothness", 0.15f);
	m_ShaderProgram.SetFloat3("u_Color", glm::vec3{ 0.0f});
	m_ShaderProgram.SetFloat3("u_CameraPos", m_Camera.GetPosition());
	m_ShaderProgram.SetFloat("u_SpotLight.Attenuation.Constant", m_SpotLight.GetAttenuation().Constant);
	m_ShaderProgram.SetFloat("u_SpotLight.Attenuation.Linear", m_SpotLight.GetAttenuation().Linear);
	m_ShaderProgram.SetFloat("u_SpotLight.Attenuation.Quadratic", m_SpotLight.GetAttenuation().Quadratic);
	m_ShaderProgram.SetFloat3("u_SpotLight.Color.Diffuse", m_SpotLight.GetColor().Diffuse);
	m_ShaderProgram.SetFloat3("u_SpotLight.Color.Specular", m_SpotLight.GetColor().Specular);
	m_ShaderProgram.SetFloat3("u_SpotLight.Color.Ambient", m_SpotLight.GetColor().Ambient);
	m_ShaderProgram.SetFloat3("u_SpotLight.Position", m_SpotLight.GetPosition());
	m_ShaderProgram.SetFloat3("u_SpotLight.Direction", m_SpotLight.GetDirection());
	m_ShaderProgram.SetFloat("u_SpotLight.CutOff", m_SpotLight.GetCutOff());
	m_ShaderProgram.SetFloat("u_SpotLight.OuterCutOff", m_SpotLight.GetOuterCutOff());
	m_ShaderProgram.SetFloat3("u_PointLight.Position", m_PointLight.GetPosition());
	m_ShaderProgram.SetFloat3("u_PointLight.Color.Diffuse", m_PointLight.GetColor().Diffuse);
	m_ShaderProgram.SetFloat3("u_PointLight.Color.Specular", m_PointLight.GetColor().Specular);
	m_ShaderProgram.SetFloat3("u_PointLight.Color.Ambient", m_PointLight.GetColor().Ambient);
	m_ShaderProgram.SetFloat("u_PointLight.Attenuation.Constant", m_PointLight.GetAttenuation().Constant);
	m_ShaderProgram.SetFloat("u_PointLight.Attenuation.Linear", m_PointLight.GetAttenuation().Linear);
	m_ShaderProgram.SetFloat("u_PointLight.Attenuation.Quadratic", m_PointLight.GetAttenuation().Quadratic);
	m_ShaderProgram.SetFloat3("u_DirLight.Direction", m_DirLight.GetDirection());
	m_ShaderProgram.SetFloat3("u_DirLight.Color.Diffuse", m_DirLight.GetColor().Diffuse);
	m_ShaderProgram.SetFloat3("u_DirLight.Color.Specular", m_DirLight.GetColor().Specular);
	m_ShaderProgram.SetFloat3("u_DirLight.Color.Ambient", m_DirLight.GetColor().Ambient);

	glm::mat4 modelTransform = glm::mat4(1.0f);
	m_ShaderProgram.SetMat4("u_Model", modelTransform);
	m_ShaderProgram.SetMat4("u_View", m_Camera.GetViewTransform());
	m_ShaderProgram.SetMat4("u_Projection", m_Camera.GetProjectionTransform());
	m_ShaderProgram.SetMat4("u_Normal", glm::transpose(glm::inverse(modelTransform)));

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
