#pragma once

#include "Buffer.hpp"
#include "Error.hpp"
#include "Repository.hpp"
#include "StrArray.hpp"

#include <cstddef>
#include <git2/config.h>
#include <git2/global.h>
#include <git2/repository.h>
#include <git2/sys/alloc.h>
#include <git2/types.h>
#include <string>
#include <utility>

#ifdef _MSC_VER
#	include <BaseTsd.h>
using ssize_t = SSIZE_T;
#endif

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

	auto features() { return git_libgit2_features(); }

	auto maxWindowSize() { return getOpt<std::size_t>(GIT_OPT_GET_MWINDOW_SIZE); }
	auto setMaxWindowSize(std::size_t size) { setOpt(GIT_OPT_SET_MWINDOW_SIZE, size); }

	auto maxWindowMappedLimit() { return getOpt<std::size_t>(GIT_OPT_GET_MWINDOW_MAPPED_LIMIT); }
	auto setMaxWindowMappedLimit(std::size_t size) { setOpt(GIT_OPT_SET_MWINDOW_MAPPED_LIMIT, size); }

	auto maxWindowFileLimit() { return getOpt<std::size_t>(GIT_OPT_GET_MWINDOW_FILE_LIMIT); }
	auto setMaxWindowFileLimit(std::size_t size) { setOpt(GIT_OPT_SET_MWINDOW_FILE_LIMIT, size); }

	auto searchPath(git_config_level_t level) { return getOpt(GIT_OPT_GET_SEARCH_PATH, level); }
	auto setSearchPath(git_config_level_t level, const std::string& path) { setOpt(GIT_OPT_SET_SEARCH_PATH, level, path.c_str()); }

	auto setCacheObjectLimit(git_object_t type, std::size_t size) { setOpt(GIT_OPT_SET_CACHE_OBJECT_LIMIT, type, size); }
	auto setCacheMaxSize(ssize_t max_storage_bytes) { setOpt(GIT_OPT_SET_CACHE_MAX_SIZE, max_storage_bytes); }
	auto enableCaching(bool enable) { setOpt(GIT_OPT_ENABLE_CACHING, static_cast<int>(enable)); }
	auto cachedMemory()
	{
		ssize_t current, allowed;
		git_libgit2_opts(GIT_OPT_GET_CACHED_MEMORY, &current, &allowed);
		return std::tuple { current, allowed };
	}

	auto templatePath() { return getOpt(GIT_OPT_GET_TEMPLATE_PATH); }
	auto setTemplatePath(const std::string& path) { setOpt(GIT_OPT_SET_TEMPLATE_PATH, path.c_str()); }

	auto setSslCertFile(const std::string& file) { setOpt(GIT_OPT_SET_SSL_CERT_LOCATIONS, file.c_str(), nullptr); }
	auto setSslCertPath(const std::string& path) { setOpt(GIT_OPT_SET_SSL_CERT_LOCATIONS, nullptr, path.c_str()); }
	auto setSslCiphers(const std::string& ciphers) { setOpt(GIT_OPT_SET_SSL_CIPHERS, ciphers.c_str()); }

	auto userAgent() { return getOpt(GIT_OPT_GET_USER_AGENT); }
	auto setUserAgent(const std::string& user_agent) { setOpt(GIT_OPT_SET_USER_AGENT, user_agent.c_str()); }

	auto windowsShareMode() { return getOpt<unsigned long>(GIT_OPT_GET_WINDOWS_SHAREMODE); }
	auto setWindowsShareMode(unsigned long value) { setOpt(GIT_OPT_SET_WINDOWS_SHAREMODE, value); }

	auto enableStrictObjectCreation(bool enable) { setOpt(GIT_OPT_ENABLE_STRICT_OBJECT_CREATION, static_cast<int>(enable)); }
	auto enableStrictSymbolicRefCreation(bool enable) { setOpt(GIT_OPT_ENABLE_STRICT_SYMBOLIC_REF_CREATION, static_cast<int>(enable)); }
	auto enableStrictHashVerification(bool enable) { setOpt(GIT_OPT_ENABLE_STRICT_HASH_VERIFICATION, static_cast<int>(enable)); }

	auto enableOffsetDelta(bool enable) { setOpt(GIT_OPT_ENABLE_OFS_DELTA, static_cast<int>(enable)); }

	auto enableFsyncGitdir(bool enable) { setOpt(GIT_OPT_ENABLE_FSYNC_GITDIR, static_cast<int>(enable)); }

	auto setAllocator(git_allocator* allocator) { setOpt(GIT_OPT_SET_ALLOCATOR, allocator); }

	auto enableUnsavedIndexSafety(bool enable) { setOpt(GIT_OPT_ENABLE_UNSAVED_INDEX_SAFETY, static_cast<int>(enable)); }

	auto packMaxObjects() { return getOpt<std::size_t>(GIT_OPT_GET_PACK_MAX_OBJECTS); }
	auto setPackMaxObjects(std::size_t objects) { setOpt(GIT_OPT_SET_PACK_MAX_OBJECTS, objects); }

	auto disablePackKeepFileChecks(bool disable) { setOpt(GIT_OPT_DISABLE_PACK_KEEP_FILE_CHECKS, static_cast<int>(disable)); }

	auto enableHttpExpectContinue(bool enable) { setOpt(GIT_OPT_ENABLE_HTTP_EXPECT_CONTINUE, static_cast<int>(enable)); }

	auto setOdbPackedPriority(int priority) { setOpt(GIT_OPT_SET_ODB_PACKED_PRIORITY, priority); }
	auto setOdbLoosePriority(int priority) { setOpt(GIT_OPT_SET_ODB_LOOSE_PRIORITY, priority); }

	auto extensions() { return getOptStrs(GIT_OPT_GET_EXTENSIONS); }
	auto setExtensions(std::span<const char*> extensions) { setOpt(GIT_OPT_SET_EXTENSIONS, extensions.data(), extensions.size()); }

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

	private:
	template<class Out, class... Args>
	auto getOpt(git_libgit2_opt_t opt, Args... args)
	{
		Out retval;
		const auto status = git_libgit2_opts(opt, args..., &retval);
		ensureOk(status);

		return retval;
	}

	template<class... Args>
	auto getOpt(git_libgit2_opt_t opt, Args... args)
	{
		Buffer buf;
		const auto status = git_libgit2_opts(opt, args..., &buf);
		ensureOk(status);

		return buf;
	}

	template<class... Args>
	auto getOptStrs(git_libgit2_opt_t opt, Args... args)
	{
		StrArray array;
		const auto status = git_libgit2_opts(opt, args..., &array);
		ensureOk(status);

		return array;
	}

	template<class... Args>
	auto setOpt(git_libgit2_opt_t opt, Args... args)
	{
		const auto status = git_libgit2_opts(opt, args...);
		ensureOk(status);
	}
};
}
