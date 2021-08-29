#pragma once

#include "Application.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "GameCamera.h"

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
	const float m_Vertices[20] = {
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
	};

	const unsigned int m_Indices[6] = {
		0, 1, 2,
		2, 1, 3,
	};

	VertexArray m_VertexArray;
	ShaderProgram m_ShaderProgram;
	Texture m_Texture;

	GameCamera m_Camera;
};