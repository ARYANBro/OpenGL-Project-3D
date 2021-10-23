#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <forward_list>
#include <stdexcept>
#include <functional>
#include <vector>
#include <memory>
#include <cstdint>
#include <string>

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

class Shader
{
public:
	Shader() noexcept;
	explicit Shader(const std::string& name) noexcept;
	Shader(Shader&& shader) noexcept;
	Shader(const Shader&) = delete;
	~Shader() noexcept;

	void Bind() const noexcept;
	void Unbind() const noexcept;
	void AttachShader(const std::string& source, GLenum type);
	void Link();

	void SetInt(const std::string& name, int value) const;
	void SetUint(const std::string& name, std::uint_fast32_t value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetFloat3(const std::string& name, glm::vec3 value) const;
	void SetMat4(const std::string& name, glm::mat4 value) const;
	void SetName(const std::string& name) noexcept { m_Name = name; }

	std::uint_fast32_t GetRendererID() const noexcept { return m_RendererID; }
	const std::string& GetName() const noexcept { return m_Name; }

	Shader& operator=(Shader&) = delete;
	Shader& operator=(Shader&& shader) noexcept;

private:
	std::uint_fast32_t m_RendererID;
	std::forward_list<GLuint> m_Shaders;
	std::string m_Name;
};

class ShaderLibrary
{
public:
	template<typename... Args>
	static std::uint_fast32_t Create(Args&&... args);
	static Shader* FindIf(const std::function<bool(Shader&)>& condition);
	static Shader* Find(std::uint_fast32_t rendererID) noexcept;

private:
	static std::vector<Shader> s_Shaders;
};

template<typename... Args>
std::uint_fast32_t ShaderLibrary::Create(Args&&... args)
{
	return s_Shaders.emplace_back(std::forward<Args>(args)...).GetRendererID();
}