#pragma once

#include "Error.hpp"

#include <git2/errors.h>
#include <git2/repository.h>
#include <string>
#include <string_view>
#include <utility>

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

	operator bool() { return m_repo != nullptr; }
	operator git_repository*() { return m_repo; }
	auto operator&() -> git_repository** { return &m_repo; }

	private:
	git_repository* m_repo;
};

struct RepositoryInitOptions : git_repository_init_options
{
	RepositoryInitOptions()
	{ 
		const auto status = git_repository_init_options_init(this, GIT_REPOSITORY_INIT_OPTIONS_VERSION); 
		ensureOk(status);
	}
	~RepositoryInitOptions() = default;

	// shallow copy
	RepositoryInitOptions(const RepositoryInitOptions& other) = default;
	auto operator=(const RepositoryInitOptions& other) -> RepositoryInitOptions& = default;

	// shallow move
	RepositoryInitOptions(RepositoryInitOptions&& other) = default;
	auto operator=(RepositoryInitOptions&& other) -> RepositoryInitOptions& = default;

	// the following allow RepositoryInitOptions to own its strings.
	// you can still use the const char* fields if you prefer, but the strings
	// they point to need to outlive this object.

	auto setWorkdirPath(std::string workdirPath)
	{
		m_workdirPath = std::move(workdirPath);
		workdir_path  = m_workdirPath.c_str();
	}

	auto setDescription(std::string desc)
	{
		m_description = std::move(desc);
		description   = m_description.c_str();
	}

	auto setTemplatePath(std::string templatePath)
	{
		m_templatePath = std::move(templatePath);
		template_path  = m_templatePath.c_str();
	}

	auto setInitialHead(std::string initialHead)
	{
		m_initialHead = std::move(initialHead);
		initial_head  = m_initialHead.c_str();
	}

	auto setOriginUrl(std::string originUrl)
	{
		m_originUrl = std::move(originUrl);
		origin_url  = m_originUrl.c_str();
	}

	private:
	std::string m_workdirPath;
	std::string m_description;
	std::string m_templatePath;
	std::string m_initialHead;
	std::string m_originUrl;
};

}
