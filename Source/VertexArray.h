#pragma once

#include "Buffer.h"

#include <glad/glad.h>

#include <cstdint>
#include <forward_list>
#include <memory>

struct AttributeFormat
{
	int Size;
	GLenum Type;
	bool Normalized;
	unsigned int RelativeOffset;
};

class VertexArray
{
public:
	VertexArray() noexcept;
	VertexArray(const VertexArray&) = delete;
	~VertexArray() noexcept;

	VertexArray& operator=(const VertexArray&) = delete;

	void Bind() const noexcept;
	void Unbind() const noexcept;
	void BindVertexBuffer(std::size_t index, std::unique_ptr<Buffer> buffer, std::size_t stride) noexcept;
	void BindIndexBuffer(std::unique_ptr<Buffer> buffer, std::uint_fast32_t count) noexcept;
	void BindAttribute(std::size_t attributeIndex, std::size_t bufferIndex) const noexcept;
	void EnableAttribute(std::size_t attributeIndex) const noexcept;
	void DisableAttribute(std::size_t attributeIndex) const noexcept;
	void SetAttributeFormat(std::size_t attributeIndex, AttributeFormat attributeFormat) const noexcept;

	std::uint_fast32_t GetNumIndices() const;

private:
	std::uint_fast32_t m_RendererID;
	std::forward_list<std::unique_ptr<Buffer>> m_VertexBuffers;
	std::unique_ptr<Buffer> m_IndexBuffer;
};