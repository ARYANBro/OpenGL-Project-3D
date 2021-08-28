#pragma once

#include <cstdint>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <forward_list>
#include <stdexcept>

class ShaderError : public std::runtime_error
{
public:
	ShaderError(const std::string& message) noexcept	
		: std::runtime_error(message) {}
};

class ShaderCompilationError : public ShaderError
{
public:
	ShaderCompilationError(GLenum shaderType, const std::string& infoLog) noexcept
		: ShaderError("Shader failed to compile"), m_ShaderType(shaderType), m_InfoLog(infoLog) {}

	const GLenum& GetShaderType() const noexcept { return m_ShaderType; }
	const std::string& GetInfoLog() const noexcept { return m_InfoLog; }

private:
	std::string m_InfoLog;
	GLenum m_ShaderType;
};

class ShaderLinkError : public ShaderError
{
public:
	ShaderLinkError(const std::string& infoLog) noexcept
		: ShaderError("Shader Program failed to link"), m_InfoLog(infoLog) {}

	const std::string& GetInfoLog() const noexcept { return m_InfoLog; }

private:
	std::string m_InfoLog;
};

class ShaderProgram
{
public:
	ShaderProgram() noexcept;
	ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
	ShaderProgram(const ShaderProgram&) = delete;
	~ShaderProgram() noexcept;

	void Bind() const noexcept;
	void Unbind() const noexcept;
	void AttachShader(const std::string& source, GLenum type);
	void Link();

	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, glm::mat4 value) const;

	ShaderProgram& operator=(ShaderProgram&) = delete;
	ShaderProgram& operator=(ShaderProgram&& shader) noexcept;

	std::uint_fast32_t GetRendererID() const noexcept { return m_RendererID; }

private:
	std::uint_fast32_t m_RendererID;
	std::forward_list<GLuint> m_Shaders;
};