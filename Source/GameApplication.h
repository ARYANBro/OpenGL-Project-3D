#pragma once

#include "Application.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
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
	const float m_Vertices[32] = {
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f,  0.5f, -1.0f, 1.0f,
		 0.5f,  0.5f, -1.0f, 1.0f,
		-0.5f, -0.5f, -1.0f, 1.0f,
		 0.5f, -0.5f, -1.0f, 1.0f
	};

	const unsigned int m_Indices[36] = {
		0, 1, 2,
		2, 1, 3,

		4, 5, 6,
		6, 5, 7,

		0, 4, 2,
		2, 4, 6,

		1, 5, 3,
		3, 5, 7,

		0, 4, 5,
		5, 0, 1,

		2, 6, 7,
		7, 2, 3
	};

	VertexArray m_VertexArray;
	ShaderProgram m_ShaderProgram;

#if 0
	glm::dvec2 m_PreviousMousePos;
	
	Camera m_Camera;

	float m_Speed = 1.5f;
	float m_MouseSpeed = 0.15f;
#endif
	GameCamera m_Camera;
};