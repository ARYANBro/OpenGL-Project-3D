#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <memory>

int main()
{
	int glfwInitStatus = glfwInit();
	assert(glfwInitStatus == GLFW_TRUE);

	constexpr int windowWidth = 1280;
	constexpr int windowHeight = 720;
	const std::string windowTitle = "OpenGL Project 3D";

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
	assert(window != nullptr);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	glfwMakeContextCurrent(window);

	int gladInitResult = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	assert(gladInitResult != 0);

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n'
		<< "Renderer: " << glGetString(GL_RENDERER) << '\n'
		<< "Version: " << glGetString(GL_VERSION) << std::endl;

	GLuint vao;
	glCreateVertexArrays(1, &vao);

	constexpr float vertices[] = {
		-0.5f,  0.5f, 
		 0.5f,  0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f
	};

	constexpr unsigned int indices[] = {
		0, 1, 2,
		2, 1, 3
	};

	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, 2 * sizeof(float));

	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);

	GLuint ibo;
	glCreateBuffers(1, &ibo);
	glNamedBufferData(ibo, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexArrayElementBuffer(vao, ibo);

	glBindVertexArray(vao);

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

	GLuint shaderProgram = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);

	glCompileShader(vertexShader);

	{
		GLint compileStatus;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);

		if (compileStatus == GL_FALSE)
		{
			GLint infoLogLength;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);

			auto infoLog = std::make_unique<GLchar[]>(infoLogLength);
			glGetShaderInfoLog(vertexShader, infoLogLength, nullptr, infoLog.get());
			
			std::cout << "Vertex Shader failed to compile\n" << std::endl;
			std::cout << infoLog.get() << std::endl;
		}
	}

	glAttachShader(shaderProgram, vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);

	glCompileShader(fragmentShader);

	{
		GLint compileStatus;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);

		if (compileStatus == GL_FALSE)
		{
			GLint infoLogLength;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);

			auto infoLog = std::make_unique<GLchar[]>(infoLogLength);
			glGetShaderInfoLog(vertexShader, infoLogLength, nullptr, infoLog.get());

			std::cout << "Fragment Shader failed to compile\n" << std::endl;
			std::cout << infoLog.get() << std::endl;
		}
	}

	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	GLint linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

		auto infoLog = std::make_unique<GLchar[]>(infoLogLength);
		glGetProgramInfoLog(shaderProgram, infoLogLength, nullptr, infoLog.get());

		std::cout << "Shader Program failed to link\n" << std::endl;
		std::cout << infoLog.get() << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.05f, 0.05f, 1.0f);

		glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_INT, nullptr);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}