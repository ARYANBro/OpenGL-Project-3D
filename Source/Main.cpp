#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <memory>
#include <cmath>

#include "Window.h"
#include "RenderingContext.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "ShaderProgram.h"
#include "GameApplication.h"

int main()
{
	WindowProps windowProps = {
		.Width = 1280,
		.Height = 720,
		.Title = "3D Project"
	};

	GameApplication game(windowProps);
	game.Start();
}
