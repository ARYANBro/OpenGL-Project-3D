#include "ShaderProgram.h"

#include <utility>
#include <memory>

ShaderProgram::ShaderProgram() noexcept
	: m_RendererID(glCreateProgram())
{
}

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
{
	*this = std::move(shaderProgram);
}

ShaderProgram::~ShaderProgram() noexcept
{
	glDeleteProgram(m_RendererID);
}

void ShaderProgram::Bind() const noexcept
{
	glUseProgram(m_RendererID);
}

void ShaderProgram::Unbind() const noexcept
{
	glUseProgram(0);
}

void ShaderProgram::AttachShader(const std::string& source, GLenum type)
{
	GLuint shader = glCreateShader(type);
	
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		auto infoLog = std::make_unique<GLchar[]>(infoLogLength);
		glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog.get());

		using namespace std::string_literals;
		throw std::runtime_error("Shader failed to compile:\n"s + infoLog.get());
	}

	glAttachShader(m_RendererID, shader);
	m_Shaders.push_front(shader);
}

void ShaderProgram::Link()
{
	glLinkProgram(m_RendererID);

	GLint linkStatus;
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &infoLogLength);

		auto infoLog = std::make_unique<GLchar[]>(infoLogLength);
		glGetProgramInfoLog(m_RendererID, infoLogLength, nullptr, infoLog.get());

		using namespace std::string_literals;
		throw std::runtime_error("Shader Program failed to link:\n"s + infoLog.get());
	}

	for (auto& shader : m_Shaders)
		glDeleteShader(shader);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
{
	if (this != &shaderProgram)
	{
		m_RendererID = shaderProgram.m_RendererID;
		shaderProgram.m_RendererID = 0;
	}

	return *this;
}