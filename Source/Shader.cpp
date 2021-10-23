#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <utility>
#include <memory>

Shader::Shader() noexcept
	: Shader(std::string())
{
}

Shader::Shader(const std::string& name) noexcept
	: m_RendererID(glCreateProgram()), m_Name(name)
{
}

Shader::Shader(Shader&& shader) noexcept
{
	*this = std::move(shader);
}

Shader::~Shader() noexcept
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const noexcept
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const noexcept
{
	glUseProgram(0);
}

void Shader::AttachShader(const std::string& source, GLenum type)
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

void Shader::Link()
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

void Shader::SetInt(const std::string& name, int value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());

	if (location == -1)
		throw ShaderError(name + " uniform location not found");

	glProgramUniform1i(m_RendererID, location, value);
}

void Shader::SetUint(const std::string& name, std::uint_fast32_t value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());

	if (location == -1)
		throw ShaderError(name + " uniform location not found");

	glProgramUniform1ui(m_RendererID, location, value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());

	if (location == -1)
		throw ShaderError(name + " uniform location not found");

	glProgramUniform1f(m_RendererID, location, value);
}

void Shader::SetFloat3(const std::string& name, glm::vec3 value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());

	if (location == -1)
		throw ShaderError(name + " uniform location not found");

	glProgramUniform3f(m_RendererID, location, value.x, value.y, value.z);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());

	if (location == -1)
		throw ShaderError(name + " uniform location not found");

	glProgramUniformMatrix4fv(m_RendererID, location, 1, GL_FALSE, glm::value_ptr(value));
}

Shader& Shader::operator=(Shader&& shader) noexcept
{
	if (this != &shader)
	{
		m_RendererID = shader.m_RendererID;
		m_Name = shader.m_Name;

		shader.m_Name = std::string();
		shader.m_RendererID = 0;
	}

	return *this;
}

std::vector<Shader> ShaderLibrary::s_Shaders;

Shader* ShaderLibrary::Find(std::uint_fast32_t rendererID) noexcept
{
	return FindIf([rendererID](Shader& shader) { return shader.GetRendererID() == rendererID; });
}

Shader* ShaderLibrary::FindIf(const std::function<bool(Shader&)>& condition)
{
	for (Shader& shader : s_Shaders)
	{
		if (condition(shader))
			return &shader;
	}

	return nullptr;
}