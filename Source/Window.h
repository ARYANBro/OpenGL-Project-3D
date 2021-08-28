#pragma once

#include <string>

class GLFWwindow;

struct WindowProps
{
	int Width;
	int Height;
	std::string Title;
};

class Window
{
public:
	Window(const WindowProps& windowProps);
	Window(const Window&) = delete;
	~Window() noexcept;

	Window& operator=(const Window&) = delete;

	bool IsOpen() const noexcept;
	void PollEvents() const noexcept;
	void SwapBuffer() const noexcept;

	const WindowProps& GetProperties() const noexcept { return m_WindowProps; }
	GLFWwindow* GetHandle() const noexcept { return m_WindowHandle; }

private:
	WindowProps m_WindowProps;
	GLFWwindow* m_WindowHandle;
};
