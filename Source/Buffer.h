#pragma once

#include <cstdint>

class Buffer
{
public:
	Buffer(std::size_t size) noexcept;
	Buffer(const void* data, std::size_t size) noexcept;
	Buffer(const Buffer&) noexcept = delete;
	Buffer(Buffer&& buffer) noexcept;
	~Buffer() noexcept;

	void SubData(const void* data, std::size_t size, std::size_t offset) noexcept;

	Buffer& operator=(const Buffer&) = delete;
	Buffer& operator=(Buffer&& buffer) noexcept;

	std::uint_fast32_t GetRendererID() const noexcept { return m_RendererID; }
	std::size_t GetSize() const noexcept { return m_Size; }

private:
	std::uint_fast32_t m_RendererID;
	std::size_t m_Size;
};