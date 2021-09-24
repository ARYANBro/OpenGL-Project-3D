#include "Buffer.h"

#include <glad/glad.h>

#include <utility>

Buffer::Buffer(std::size_t size) noexcept
{
	glCreateBuffers(1, &m_RendererID);
	glNamedBufferStorage(m_RendererID, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

Buffer::Buffer(const void* data, std::size_t size) noexcept
	: m_Size(size)
{
	glCreateBuffers(1, &m_RendererID);
	glNamedBufferData(m_RendererID, size, data, GL_STATIC_DRAW);
}

Buffer::Buffer(Buffer&& buffer) noexcept
{
	*this = std::move(buffer);
}

Buffer::~Buffer() noexcept
{
	glDeleteBuffers(1, &m_RendererID);
}

void Buffer::SubData(const void* data, std::size_t size, std::size_t offset) noexcept
{
	glNamedBufferSubData(m_RendererID, offset, size, data);
}

Buffer& Buffer::operator=(Buffer&& buffer) noexcept
{
	if (this != &buffer)
	{
		m_RendererID = buffer.m_RendererID;
		buffer.m_RendererID = 0;
	}

	return *this;
}
