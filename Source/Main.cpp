#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <memory>

#include "Window.h"
#include "RenderingContext.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "ShaderProgram.h"

int main()
{
	Window::WindowProps windowProps;
	windowProps.Width = 1280;
	windowProps.Height = 720;
	windowProps.Title = "OpenGL Project 3D";

	Window window(windowProps);
	RenderingContext renderingCtxt(window);

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n'
		<< "Renderer: " << glGetString(GL_RENDERER) << '\n'
		<< "Version: " << glGetString(GL_VERSION) << std::endl;

	constexpr float vertices[] = {
		-0.5f,  0.5f, 
		 0.5f,  0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f
	};

	auto vertexBuffer = std::make_unique<Buffer>(vertices, sizeof(vertices));

	VertexArray vertexArray;
	vertexArray.BindVertexBuffer(0, std::move(vertexBuffer), 2 * sizeof(float));
	vertexArray.BindAttribute(0, 0);

	AttributeFormat attribFormat;
	attribFormat.Type = GL_FLOAT;
	attribFormat.Size = 2;
	attribFormat.RelativeOffset = 0;
	attribFormat.Normalized = false;

	vertexArray.SetAttributeFormat(0, attribFormat);

	constexpr unsigned int indices[] = {
		0, 1, 2,
		2, 1, 3
	};

	auto indexBuffer = std::make_unique<Buffer>(indices, sizeof(indices));
	vertexArray.BindIndexBuffer(std::move(indexBuffer), std::size(indices));

	const char* vertexShaderSrc = R"(
		#version 460

		layout(location = 0) in vec4 a_VertexPos;

		void main()
		{
			gl_Position = a_VertexPos;
		}
	)";

	const char* fragmentShaderSrc = R"(
		#version 460

		out vec4 glFragColor;

		void main()
		{
			glFragColor = vec4(0.9f, 0.2f, 0.2f, 1.0f);
		}
	)";

	ShaderProgram shader;
	shader.AttachShader(vertexShaderSrc, GL_VERTEX_SHADER);
	shader.AttachShader(fragmentShaderSrc, GL_FRAGMENT_SHADER);
	shader.Link();

	glfwSwapInterval(1);

	vertexArray.EnableAttribute(0);
	vertexArray.Bind();
	shader.Bind();

	while (window.IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.05f, 0.05f, 1.0f);

		glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_INT, nullptr);

		window.PollEvents();
		window.SwapBuffer();
	}
}