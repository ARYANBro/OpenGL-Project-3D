#include "Application.h"

#include "DeltaTime.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

Application* Application::s_Instance = nullptr;

Application::Application(const WindowProps& windowProps)
	: m_Window(windowProps), m_RenderingCtxt(m_Window)
{	
	s_Instance = this;

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n'
		<< "Renderer: " << glGetString(GL_RENDERER) << '\n'
		<< "Version: " << glGetString(GL_VERSION) << std::endl;
}

void Application::Start()
{
	OnBegin();

	while (m_Window.IsOpen())
	{	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		glClearColor(0.05, 0.05f, 0.05f, 1.0f);

		OnUpdate(DeltaTime::Process());
		OnRender();

		m_Window.PollEvents();
		m_Window.SwapBuffer();
	}

	OnEnd();
}