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
	const float m_Vertices[72] = {
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

	const float m_TextureCoords[48] = {
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

	const float m_Normals[72] = {
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

	const unsigned int m_Indices[36] = {
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

	VertexArray m_VertexArray;
	ShaderProgram m_ShaderProgram;
	Texture m_DiffuseTex;
	Texture m_SpecularTex;

	GameCamera m_Camera;
};