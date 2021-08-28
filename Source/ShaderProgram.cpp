#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

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
		throw ShaderCompilationError(type, infoLog.get());
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
		throw ShaderLinkError(infoLog.get());
	}

	for (auto& shader : m_Shaders)
		glDeleteShader(shader);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());

	if (location == -1)
		throw ShaderError(name + " uniform location not found");

	glProgramUniform1f(m_RendererID, location, value);
}

void ShaderProgram::SetMat4(const std::string& name, glm::mat4 value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());

	if (location == -1)
		throw ShaderError(name + " uniform location not found");

	glProgramUniformMatrix4fv(m_RendererID, location, 1, GL_FALSE, glm::value_ptr(value));
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