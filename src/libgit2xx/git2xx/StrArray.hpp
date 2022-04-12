#pragma once

#include "Error.hpp"

#include <span>
#include <utility>

#include <git2/strarray.h>

namespace Git
{
class StrArray
{
	public:
	StrArray():
	    m_array { }
	{ }

	~StrArray()
	{
		git_strarray_dispose(&m_array);
	}

	StrArray(const StrArray& other)
	{
		const auto status = git_strarray_copy(&m_array, &other.m_array);
		ensureOk(status);
	}

	auto operator=(const StrArray& other) -> StrArray&
	{
		git_strarray newarray;
		const auto status = git_strarray_copy(&newarray, &other.m_array);
		ensureOk(status);

		git_strarray_dispose(&m_array);
		m_array = newarray;

		return *this;
	}

	StrArray(StrArray&& other) noexcept:
	    m_array { std::exchange(other.m_array, {}) }
	{ }

	auto operator=(StrArray&& other) noexcept -> StrArray&
	{
		std::swap(m_array, other.m_array);
		return *this;
	}

	auto operator&() -> git_strarray* { return &m_array; }

	auto data() { return std::span { m_array.strings, m_array.count }; }

	private:
	git_strarray m_array;
};
}
