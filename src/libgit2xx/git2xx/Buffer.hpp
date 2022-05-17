#pragma once

#include <git2/buffer.h>
#include <span>
#include <string_view>
#include <utility>

namespace Git
{
class Buffer
{
	public:
	Buffer():
	    m_buf {}
	{ }

	Buffer(git_buf buf):
	    m_buf { buf }
	{ }

	~Buffer() { git_buf_dispose(&m_buf); }

	Buffer(const Buffer& other) = delete;
	auto operator=(const Buffer& other) -> Buffer& = delete;

	Buffer(Buffer&& other) noexcept:
	    m_buf { std::exchange(other.m_buf, {}) }
	{ }

	auto operator=(Buffer&& other) noexcept -> Buffer&
	{
		std::swap(m_buf, other.m_buf);
		return *this;
	}

	auto operator&() -> git_buf* { return &m_buf; }

	auto data() { return std::span {m_buf.ptr, m_buf.size}; }
	auto data() const -> const std::span<char> { return std::span {m_buf.ptr, m_buf.size}; }
	auto view() { return std::string_view { m_buf.ptr, m_buf.size }; }
	auto view() const -> const std::string_view { return std::string_view { m_buf.ptr, m_buf.size }; }

	private:
	git_buf m_buf;
};
}
