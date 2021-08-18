#pragma once

#include <cstdint>
#include <string>

#include <glad/glad.h>

#include <forward_list>

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

	ShaderProgram& operator=(ShaderProgram&) = delete;
	ShaderProgram& operator=(ShaderProgram&& shader) noexcept;

	std::uint_fast32_t GetRendererID() const noexcept { return m_RendererID; }

private:
	std::uint_fast32_t m_RendererID;
	std::forward_list<GLuint> m_Shaders;
};