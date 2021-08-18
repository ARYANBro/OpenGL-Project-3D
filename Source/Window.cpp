#include "Window.h"

#include <GLFW/glfw3.h>

#include <stdexcept>

Window::Window(const WindowProps& windowProps)
	: m_WindowProps(windowProps)
{
	int glfwInitResult = glfwInit();

	if (!glfwInitResult)
		throw std::runtime_error("Could not initialize glfw");

	m_WindowHandle = glfwCreateWindow(m_WindowProps.Width, m_WindowProps.Height, m_WindowProps.Title.c_str(), nullptr, nullptr);

	if (!m_WindowHandle)
		throw std::runtime_error("Could not create glfw window");
}

Window::~Window() noexcept
{
	glfwDestroyWindow(m_WindowHandle);
	glfwTerminate();
}

bool Window::IsOpen() const noexcept
{
	return !glfwWindowShouldClose(m_WindowHandle);
}

void Window::PollEvents() const noexcept
{
	glfwPollEvents();
}

void Window::SwapBuffer() const noexcept
{
	glfwSwapBuffers(m_WindowHandle);
}