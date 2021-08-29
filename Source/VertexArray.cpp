#include "VertexArray.h"

#include <cassert>

VertexArray::VertexArray() noexcept
{
	glCreateVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() noexcept
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const noexcept
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const noexcept
{
	glBindVertexArray(0);
}

void VertexArray::BindVertexBuffer(std::size_t index, std::unique_ptr<Buffer> buffer, std::size_t stride) noexcept
{
	assert(buffer != nullptr);

	glVertexArrayVertexBuffer(m_RendererID, index, buffer->GetRendererID(), 0, stride);
	m_VertexBuffers.push_front(std::move(buffer));
}

void VertexArray::BindIndexBuffer(std::unique_ptr<Buffer> buffer, std::uint_fast32_t count) noexcept
{
	glVertexArrayElementBuffer(m_RendererID, buffer->GetRendererID());
	m_IndexBuffer = std::move(buffer);
}

void VertexArray::BindAttribute(std::size_t attributeIndex, std::size_t bufferIndex) const noexcept
{
	glVertexArrayAttribBinding(m_RendererID, attributeIndex, bufferIndex);
}

void VertexArray::EnableAttribute(std::size_t attributeIndex) const noexcept
{
	glEnableVertexArrayAttrib(m_RendererID, attributeIndex);
}

void VertexArray::DisableAttribute(std::size_t attributeIndex) const noexcept
{
	glDisableVertexArrayAttrib(m_RendererID, attributeIndex);
}

void VertexArray::SetAttributeFormat(std::size_t attributeIndex, AttributeFormat attributeFormat) const noexcept
{
	glVertexArrayAttribFormat(m_RendererID, attributeIndex, attributeFormat.Size, attributeFormat.Type, attributeFormat.Normalized, attributeFormat.RelativeOffset);
}
