#pragma once

#include "Error.hpp"

#include <cstddef>
#include <git2/oid.h>
#include <string>
#include <string_view>

namespace Git
{

struct Oid : git_oid
{
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

	static auto fromString(std::string_view str)
	{
		Oid oid;
		const auto status = git_oid_fromstrn(&oid, str.data(), str.size());
		ensureOk(status);

		return oid;
	}

	static auto fromRaw(const unsigned char* raw)
	{
		Oid oid;
		const auto status = git_oid_fromraw(&oid, raw);
		ensureOk(status);

		return oid;
	}
};

}
