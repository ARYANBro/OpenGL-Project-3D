#pragma once

#include "Application.h"
#include "VertexArray.h"
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
	std::size_t m_NumVertexIndex;

	PointLight m_PointLight;
	VertexArray m_VertexArray;
	ShaderProgram m_ShaderProgram;
	Texture m_DiffuseTex;
	Texture m_SpecularTex;

	GameCamera m_Camera;
};