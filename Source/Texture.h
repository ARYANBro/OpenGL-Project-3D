#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>
#include <vector>
#include <functional>
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
	const std::string& GetFilePath() const noexcept { return m_FilePath; }
	std::uint_fast32_t GetRendererID() const noexcept { return m_RendererID; }
	std::uint_fast32_t GetTextureUnit() const noexcept { return m_TextureUnit; }

	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) noexcept;

private:
	int m_Width;
	int m_Height;
	int m_NumColorChannels;
	std::string m_FilePath;
	std::uint_fast32_t m_RendererID;
	mutable std::uint_fast32_t m_TextureUnit;
};

class TextureLibrary
{
public:
	template<typename... Args>
	static std::uint_fast32_t Create(Args&&... args);

	static Texture* Find(std::uint_fast32_t rendererID) noexcept;
	static Texture* FindIf(const std::function<bool(Texture&)>& condition);

private:
	static std::vector<Texture> s_Textures;
};

template<typename... Args>
std::uint_fast32_t TextureLibrary::Create(Args&&... args)
{
	return s_Textures.emplace_back(std::forward<Args>(args)...).GetRendererID();
}
