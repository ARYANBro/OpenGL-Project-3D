#pragma once

#include "Application.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "GameCamera.h"
#include "Lights.h"

#include <glm/glm.hpp>

class GameApplication : public Application
{
public:
	GameApplication(const WindowProps& windowProps)
		: Application(windowProps) {}

	void OnBegin() override;
	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnEnd() override;

private:
	// Mesh m_Mesh;

	PointLight m_PointLight;
	DirectionalLight m_DirLight;
	SpotLight m_SpotLight;
	ShaderProgram m_ShaderProgram;
	Texture m_DiffuseTex;
	Texture m_SpecularTex;
	Texture m_NormalMap;
	Texture m_RoughnessTex;
	GameCamera m_Camera;

	std::vector<Mesh> m_Meshes;
};