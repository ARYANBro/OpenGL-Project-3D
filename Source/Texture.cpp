#include "Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <utility>
#include <cassert>

Texture::Texture() noexcept
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	stbi_set_flip_vertically_on_load(true);

	SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Texture::Texture(const std::string& filePath)
	: Texture()
{
	Load(filePath);
}

Texture::Texture(Texture&& texture)
{
	*this = std::move(texture);
}

Texture::~Texture() noexcept
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Load(const std::string& filePath)
{
	m_FilePath = filePath;
	stbi_uc* pixels = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_NumColorChannels, 0);

	if (!pixels)
		throw TextureLoadError(filePath);

	GLenum internalFormat;
	GLenum dataFormat;

	switch (m_NumColorChannels)
	{
		case 1:
			internalFormat = GL_R8;
			dataFormat = GL_RED;
			break;
		
		case 2:
			internalFormat = GL_RG8;
			dataFormat = GL_RG;
			break;
		
		case 3:
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			break;

		case 4:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			break;

		default:
			assert(false);
	}

	glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, pixels);
	glGenerateTextureMipmap(m_RendererID);

	stbi_image_free(pixels);
}
 
void Texture::SetParameter(unsigned int parameterName, int value) const noexcept
{
	glTextureParameteri(m_RendererID, parameterName, value);
}

void Texture::Bind(unsigned int textureUnit) const noexcept
{
	m_TextureUnit = textureUnit;
	glBindTextureUnit(textureUnit, m_RendererID);	
}

void Texture::Unbind() const noexcept
{
	glBindTextureUnit(m_TextureUnit, 0);
}

Texture& Texture::operator=(Texture&& texture) noexcept
{
	if (this != &texture)
	{
		m_RendererID = texture.m_RendererID;
		m_Width = texture.m_Width;
		m_Height = texture.m_Height;
		m_NumColorChannels = texture.m_NumColorChannels;
		m_FilePath = texture.m_FilePath;

		texture.m_RendererID = 0;
	}

	return *this;
}

std::vector<Texture> TextureLibrary::s_Textures;

Texture* TextureLibrary::Find(std::uint_fast32_t rendererID) noexcept
{
	return FindIf([rendererID](Texture& texture) { return texture.GetRendererID() == rendererID; });
}

Texture* TextureLibrary::FindIf(const std::function<bool(Texture&)>& function)
{
	for (auto& texture : s_Textures)
	{
		if (function(texture))
			return &texture;
	}

	return nullptr;
}