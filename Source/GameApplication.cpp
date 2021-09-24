#include "GameApplication.h"

#include "Buffer.h"
#include "Window.h"
#include "RenderingContext.h"
#include "FileParser.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>


static std::pair<std::vector<Vertex>, std::vector<unsigned int>> ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		aiVector3D aiVertex = mesh->mVertices[i];

		vertex.VertexPos.x = aiVertex.x;
		vertex.VertexPos.y = aiVertex.y;
		vertex.VertexPos.z = aiVertex.z;

		aiVector3D aiNormal = mesh->mNormals[i];

		vertex.Normal.x = aiNormal.x;
		vertex.Normal.y = aiNormal.y;
		vertex.Normal.z = aiNormal.z;

		assert(mesh->mTextureCoords[0] != nullptr);

		aiVector3D aiTextureCoord = mesh->mTextureCoords[0][i];

		vertex.TextureCoord.x = aiTextureCoord.x;
		vertex.TextureCoord.y = aiTextureCoord.y;

		aiVector3D aiTangent = mesh->mTangents[0];

		vertex.Tangent.x = aiTangent.x;
		vertex.Tangent.y = aiTangent.y;
		vertex.Tangent.z = aiTangent.z;

		vertices.push_back(vertex);
	}

	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	return { vertices, indices };
}

static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& inMeshes)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		auto [processedVertices, processedIndices] = ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene);
		inMeshes.emplace_back(processedVertices, processedIndices);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		ProcessNode(node->mChildren[i], scene, inMeshes);
}

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

	const std::vector<glm::vec3> vertexPositions = {
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

	const std::vector<glm::vec2> textureCoords = {
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

	const std::vector<glm::vec3> normals = {
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

	const std::vector<unsigned int> indices = {
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

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("Assets\\3DModels\\Backpack\\backpack.obj", aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	assert(scene != nullptr && scene->mRootNode != nullptr);

	aiMesh* mesh = scene->mMeshes[scene->mRootNode->mChildren[0]->mMeshes[0]];
	assert(mesh != nullptr);

	ProcessNode(scene->mRootNode, scene, m_Meshes);

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
		m_DiffuseTex.Load("Assets\\3DModels\\Backpack\\diffuse.jpg");
		m_SpecularTex.Load("Assets\\3DModels\\Backpack\\specular.jpg");
		m_NormalMap.Load("Assets\\3DModels\\Backpack\\normal.png");
		m_RoughnessTex.Load("Assets\\3DModels\\Backpack\\roughness.jpg");
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
	m_RoughnessTex.Bind(3);

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
	m_ShaderProgram.SetInt("u_RoughnessTex", 3);
	m_ShaderProgram.SetFloat3("u_Color", glm::vec3{ 0.0f});
	m_ShaderProgram.SetFloat3("u_CameraPos", m_Camera.GetPosition());
	m_ShaderProgram.SetUint("u_NumActivePointLights", 1);
	m_ShaderProgram.SetUint("u_NumActiveSpotLights", 1);
	m_ShaderProgram.SetUint("u_NumActiveDirLights", 1);

	for (Mesh& mesh : m_Meshes)
	{
		mesh.GetVertexArray().Bind();
		glDrawElements(GL_TRIANGLES, mesh.GetNumVertices(), GL_UNSIGNED_INT, nullptr);
	}

	// m_DiffuseTex.Unbind();
	// m_RoughnessTex.Unbind();
	// m_NormalMap.Unbind();

	// m_ShaderProgram.SetMat4("u_Model", glm::scale(glm::translate(glm::mat4(1.0f), m_PointLight.GetPosition()), glm::vec3(0.2f)));
	// m_ShaderProgram.SetFloat3("u_Color", glm::vec3{ 1.0f});

	// for (Mesh& mesh : m_Meshes)
	// {
	// 	mesh.GetVertexArray().Bind();
	// 	glDrawElements(GL_TRIANGLES, mesh.GetNumVertices(), GL_UNSIGNED_INT, nullptr);
	// }
}

void GameApplication::OnEnd()
{
}
