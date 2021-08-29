#pragma once

#include <cstdint>
#include <string>

class Texture
{
public:
	Texture() noexcept;
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