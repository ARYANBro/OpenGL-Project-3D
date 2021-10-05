#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <memory>

class TextureError : public std::runtime_error
{
public:
	TextureError(const std::string& message)
		: std::runtime_error(message) {}
};

class TextureLoadError : public TextureError
{
public:
	TextureLoadError(const std::string& fileName)
		: TextureError("Failed to load texture file"), m_FileName(fileName) {}

	const std::string& GetFileName() const noexcept { return m_FileName; }

private:
	std::string m_FileName;
};

class Texture
{
public:
	Texture() noexcept;
	explicit Texture(const std::string& filePath);
	Texture(const Texture&) = delete;
	Texture(Texture&& texture);
	~Texture() noexcept;

	void Load(const std::string& filePath);
	void SetParameter(unsigned int parameterName, int value) const noexcept;

	void Bind(unsigned int textureUnit) const noexcept;
	void Unbind() const noexcept;

	int GetWidth() const noexcept { return m_Width; }
	int GetHeight() const noexcept { return m_Height; }
	int GetNumColorChannels() const noexcept { return m_NumColorChannels; }
	std::uint_fast32_t GetRendererID() const noexcept { return m_RendererID; }

	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) noexcept;

private:
	int m_Width;
	int m_Height;
	int m_NumColorChannels;
	std::uint_fast32_t m_RendererID;
};

class TextureLibrary
{
public:
	static std::shared_ptr<Texture> Load(const std::string& path) noexcept;
	static std::shared_ptr<Texture> Find(const std::string& path) noexcept;
private:
	static std::unordered_map<std::string, std::shared_ptr<Texture>> s_Textures;
};