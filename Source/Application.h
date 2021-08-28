#pragma once

#include "Window.h"
#include "RenderingContext.h"

class Application
{
public:
	Application(const WindowProps& windowProps);

	virtual void OnBegin() {}
	virtual void OnUpdate(float deltaTime) {}
	virtual void OnRender() {}
	virtual void OnEnd() {}

	void Start();

	const Window& GetWindow() const noexcept { return m_Window; }
	const RenderingContext& GetRenderigContext() const noexcept { return m_RenderingCtxt; }

	static Application& GetInstance() noexcept { return *s_Instance; }

private:
	Window m_Window;
	RenderingContext m_RenderingCtxt;
	static Application* s_Instance;
};