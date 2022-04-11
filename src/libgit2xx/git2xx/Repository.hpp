#pragma once

#include <utility>

#include <git2/errors.h>
#include <git2/repository.h>
#include <string_view>

#include "Error.hpp"

namespace Git
{
class Repository
{
	public:
	Repository(git_repository* repo = nullptr):
	    m_repo { repo }
	{ }

	~Repository()
	{
		git_repository_free(m_repo);
	}

	Repository(const Repository& other) = delete;
	auto operator=(const Repository& other) -> Repository& = delete;

	Repository(Repository&& other) noexcept:
	    m_repo { std::exchange(other.m_repo, nullptr) }
	{ }

	auto operator=(Repository&& other) noexcept -> Repository&
	{
		std::swap(m_repo, other.m_repo);
		return *this;
	}

	operator git_repository*() { return m_repo; }
	auto operator&() -> git_repository** { return &m_repo; }

	private:
	git_repository* m_repo;
};

}
