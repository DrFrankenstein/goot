#pragma once

#include "Buffer.hpp"
#include "Error.hpp"
#include "Repository.hpp"
#include "StrArray.hpp"

#include <cstddef>
#include <git2/global.h>
#include <git2/repository.h>
#include <string>
#include <utility>

namespace Git
{
/**
 * A RAII wrapper around libgit2's init and shutdown functions, and a central
 * entry point to the library.
 */
class Git
{
	public:
	Git() { git_libgit2_init(); }
	~Git() { git_libgit2_shutdown(); }

	// we _could_ allow copying since libgit2 uses refcounting, but it might
	// also lead to too many unnecessary calls to init and shutdown if best
	// practices aren't followed.
	Git(const Git& other) = delete;
	auto operator=(const Git& other) -> Git& = delete;

	// no moving: there's no such thing as a null instance of this class, so
	// destroying the donor instance will still cause a call to shutdown.
	Git(Git&& other) = delete;
	auto operator=(Git&& other) -> Git& = delete;

	auto openRepository(
	    const std::string& path,
	    git_repository_open_flag_t flags = GIT_REPOSITORY_OPEN_NO_SEARCH,
	    const std::string& ceiling_dirs  = {})
	{
		Repository repo;
		const auto status = git_repository_open_ext(
		    &repo,
		    path.c_str(),
		    flags,
		    ceiling_dirs.empty() ? nullptr : ceiling_dirs.c_str());

		ensureOk(status);

		return repo;
	}

	auto initRepository(const std::string& path, git_repository_init_options& options)
	{
		Repository repo;
		const auto status = git_repository_init_ext(&repo, path.c_str(), &options);
		ensureOk(status);

		return repo;
	}

	auto discoverRepository(const std::string& path, bool across_fs = false, const std::string& ceiling_dirs = {})
	{
		Buffer foundPath;
		const auto status = git_repository_discover(&foundPath, path.c_str(), across_fs, ceiling_dirs.c_str());

		// in the case of ENOTFOUND, just return an empty path
		// git_repository_discover is the only way to test for an existing repo
		if (status != GIT_OK && status != GIT_ENOTFOUND)
			throw Error { status };

		return foundPath;
	}
};
}
