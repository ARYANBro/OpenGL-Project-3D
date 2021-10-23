#include "ModelLoader3D.h"

#include "Texture.h"
#include "Shader.h"
#include "FileParser.h"
#include "Renderer.h"

#include <functional>
#include <iostream>

Assimp::Importer ModelLoader3D::s_Importer;

RenderObject ModelLoader3D::ReadFile(const std::string& filePath, unsigned int flags)
{
	const aiScene* scene = s_Importer.ReadFile(filePath, flags);

	if (scene == nullptr || scene->mRootNode == nullptr)
		throw ModelLoadError("Could not read file " + filePath);

	std::size_t lastSlash = filePath.find_last_of("/\\");
	std::size_t lastDot = filePath.find_last_of('.');
	std::string fileName(filePath, lastSlash + 1, lastDot - lastSlash - 1);
	RenderObject renderObj;

	ProcessNode(filePath, scene->mRootNode, scene, renderObj);
	return renderObj;
}

void ModelLoader3D::ProcessNode(const std::string& filePath, aiNode* node, const aiScene* scene, RenderObject& inRenderObj) noexcept
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		auto model = ProcessMesh(filePath, scene->mMeshes[node->mMeshes[i]], scene);
		inRenderObj.AddModel(std::move(model));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		ProcessNode(filePath, node->mChildren[i], scene, inRenderObj);
}

Model ModelLoader3D::ProcessMesh(const std::string& filePath, aiMesh* mesh, const aiScene* scene) noexcept
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

	if (mesh->mMaterialIndex <= 0)
		assert("Material not found");

	Shader* shader = ShaderLibrary::Find(Renderer::GetShaderID());

	aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
	aiString texturePath;

	std::string directory = filePath.substr(0, filePath.find_last_of("/\\"));

	Mesh m(vertices, indices, "Unnamed Mesh");

	Material material(shader->GetRendererID());
	
	if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);

		Texture* texture = TextureLibrary::FindIf([&](Texture& texture)
		{
			return texture.GetFilePath() == std::string(directory + '/' + texturePath.C_Str());
		});

		if (texture == nullptr)
		{
			std::uint_fast32_t textureID = TextureLibrary::Create(directory + '/' + texturePath.C_Str());
			texture = TextureLibrary::Find(textureID);
		}

		material.SetDiffuseTexture(texture->GetRendererID());
	}

	if (aiMat->GetTextureCount(aiTextureType_SPECULAR) > 0)
	{
		aiMat->GetTexture(aiTextureType_SPECULAR, 0, &texturePath);

		Texture* texture = TextureLibrary::FindIf([&](Texture& texture)
		{
			return texture.GetFilePath() == std::string(directory + '/' + texturePath.C_Str());
		});

		if (texture == nullptr)
		{
			std::uint_fast32_t textureID = TextureLibrary::Create(directory + '/' + texturePath.C_Str());
			texture = TextureLibrary::Find(textureID);
		}

		material.SetSpecularTexture(texture->GetRendererID());
	}

	if (aiMat->GetTextureCount(aiTextureType_HEIGHT) > 0)
	{
		aiMat->GetTexture(aiTextureType_HEIGHT, 0, &texturePath);

		Texture* texture = TextureLibrary::FindIf([&](Texture& texture)
		{
			return texture.GetFilePath() == std::string(directory + '/' + texturePath.C_Str());
		});

		if (texture == nullptr)
		{
			std::uint_fast32_t textureID = TextureLibrary::Create(directory + '/' + texturePath.C_Str());
			texture = TextureLibrary::Find(textureID);
		}

		material.SetNormalMap(texture->GetRendererID());
	}

	aiColor3D color;
	float shininess;

	aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	material.SetDiffuse(glm::vec3(color.r, color.g, color.b));

	aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	material.SetAmbient(glm::vec3(color.r, color.g, color.b));

	aiMat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	material.SetSpecular(glm::vec3(color.r, color.g, color.b));

	aiMat->Get(AI_MATKEY_SHININESS, shininess);
	material.SetSpecularExponent(shininess);

	return Model(std::move(m), material);
}