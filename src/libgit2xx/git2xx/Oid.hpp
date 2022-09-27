#pragma once

#include "Error.hpp"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <git2/oid.h>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>

namespace Git
{

struct Oid : git_oid
{
	Oid()
	{
		std::ranges::fill(id, unsigned char {});
	}

	Oid(std::string_view str)
	{
		const auto status = git_oid_fromstrn(this, str.data(), str.size());
		ensureOk(status);
	}

	Oid(const unsigned char* raw)
	{
		const auto status = git_oid_fromraw(this, raw);
		ensureOk(status);
	}

	auto operator==(const Oid& other) const noexcept
	{
		return git_oid_equal(this, &other);
	}

	auto operator<=>(const Oid& other) const noexcept
	{
		return git_oid_cmp(this, &other);
	}

	auto compare_n(const Oid& other, std::size_t n) const noexcept
	{
		return git_oid_ncmp(this, &other, n);
	}

	auto isZero() const noexcept
	{
		const auto result = git_oid_is_zero(this);
		return static_cast<bool>(result);
	}

	auto format() const
	{
		std::string result(40, char {});
		const auto status = git_oid_nfmt(result.data(), result.size(), this);
		ensureOk(status);

		return result;
	}
};

class OidShortener
{
	public:
	OidShortener(std::size_t min_length):
	    m_shortener { git_oid_shorten_new(min_length) }
	{
		if (m_shortener == nullptr)
		{
			throw std::bad_alloc {};
		}
	}

	~OidShortener()
	{
		git_oid_shorten_free(m_shortener);
	}

	OidShortener(const OidShortener& other) = delete;
	auto operator=(const OidShortener other) -> OidShortener& = delete;

	OidShortener(OidShortener&& other) noexcept:
	    m_shortener { std::exchange(other.m_shortener, nullptr) }
	{ }
	auto operator=(OidShortener& other) noexcept -> OidShortener&
	{
		std::swap(m_shortener, other.m_shortener);
		return *this;
	}

	auto add(const std::string& text_id)
	{
		const auto result = git_oid_shorten_add(m_shortener, text_id.c_str());
		ensureOk(result);

		return result;
	}

	private:
	git_oid_shorten* m_shortener;
};
}
