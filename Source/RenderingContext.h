#pragma once

class Window;

class RenderingContext
{
public:
	explicit RenderingContext(Window& window);

private:
	Window& m_Window;
};