#pragma once

#include "Buffer.hpp"
#include "Error.hpp"

#include <git2/errors.h>
#include <git2/repository.h>
#include <string>
#include <string_view>
#include <utility>

namespace Git
{
class Git;

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

	static auto open(
	    [[maybe_unused]] const Git& git,
	    const std::string& path,
	    git_repository_open_flag_t flags = GIT_REPOSITORY_OPEN_NO_SEARCH,
	    const std::string& ceiling_dirs = {}
	)
	{
		git_repository* ptr = nullptr;
		const auto status = git_repository_open_ext(
		    &ptr,
		    path.c_str(),
		    flags,
		    ceiling_dirs.empty() ? nullptr : ceiling_dirs.c_str()
		);

		Repository repo { ptr };
		ensureOk(status);

		return repo;
	}

	static auto init(
	    [[maybe_unused]] const Git& git,
	    const std::string& path,
	    git_repository_init_options& options
	)
	{
		git_repository* ptr = nullptr;
		const auto status = git_repository_init_ext(&ptr, path.c_str(), &options);

		Repository repo { ptr };
		ensureOk(status);

		return repo;
	}

	static auto discover(
	    [[maybe_unused]] const Git& git,
	    const std::string& path,
	    bool across_fs = false,
	    const std::string& ceiling_dirs = {}
	)
	{
		Buffer foundPath;
		const auto status = git_repository_discover(&foundPath, path.c_str(), across_fs, ceiling_dirs.c_str());

		// in the case of ENOTFOUND, just return an empty path
		// git_repository_discover is the only way to test for an existing repo
		if (status != GIT_OK && status != GIT_ENOTFOUND)
			throw Error { status };

		return foundPath;
	}

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
