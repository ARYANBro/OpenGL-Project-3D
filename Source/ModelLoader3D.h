#pragma once

#include "RenderObject.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdexcept>

class ModelLoadError : public std::runtime_error
{
public:
	ModelLoadError(const std::string& what) noexcept
		: std::runtime_error(what) {}
};

class ModelLoader3D
{
public:
	static RenderObject ReadFile(const std::string& filePath, unsigned int flags = 0);
	static Assimp::Importer& GetImporter() noexcept { return s_Importer; }

private:
	static Assimp::Importer s_Importer;

private:
	static void ProcessNode(const std::string& filePath, aiNode* node, const aiScene* scene, RenderObject& inRenderObj) noexcept;
	static Model ProcessMesh(const std::string& filePath, aiMesh* mesh, const aiScene* scene) noexcept;
};
