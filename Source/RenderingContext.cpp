#include "RenderingContext.h"

#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>

RenderingContext::RenderingContext(Window& window)
	: m_Window(window)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	glfwMakeContextCurrent(window.GetHandle());

	int gladInitResult = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	if (gladInitResult == 0)
		throw std::runtime_error("Could not load glad");
}