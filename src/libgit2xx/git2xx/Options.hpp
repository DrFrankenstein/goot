#pragma once

#include "Buffer.hpp"
#include "Error.hpp"
#include "Git.hpp"
#include "StrArray.hpp"

#include <git2/config.h>
#include <git2/sys/alloc.h>
#include <git2/types.h>
#include <span>
#include <string>

#ifdef _MSC_VER
#	include <BaseTsd.h>
using ssize_t = SSIZE_T;
#endif

namespace Git
{

class Options
{
	Options(Git& git):
	    m_git { git }
	{ }

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

	Git& m_git;
};

}