#pragma once

#include "Buffer.hpp"
#include "Error.hpp"
#include "Oid.hpp"

#include <git2/errors.h>
#include <git2/refs.h>
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

	auto commonDir() const
	{
		const auto dir = git_repository_commondir(m_repo);
		return std::string_view { dir };
	}

	auto detachHead()
	{
		const auto status = git_repository_detach_head(m_repo);
		ensureOk(status);
	}

	// TODO: fetch heads

	auto getNamespace()
	{
		const auto ns = git_repository_get_namespace(m_repo);
		return std::string_view { ns };
	}

	auto setNamespace(const std::string& ns)
	{
		const auto status = git_repository_set_namespace(m_repo, ns.c_str());
		ensureOk(status);
	}

	auto hashFile(const std::string& path, git_object_t type)
	{
		Oid oid;
		const auto status = git_repository_hashfile(&oid, m_repo, path.c_str(), type, nullptr);
		ensureOk(status);

		return oid;
	}

	auto hashFile(const std::string& path, git_object_t type, const std::string& as_path)
	{
		Oid oid;
		const auto status = git_repository_hashfile(&oid, m_repo, path.c_str(), type, as_path.c_str());
		ensureOk(status);

		return oid;
	}

	// These return raw git_reference* to avoid a circular dependency between
	// Repository.hpp and Reference.hpp.
	// You may (and should) make your own Repository objects out of them.

	auto head()
	{
		git_reference* ref = nullptr;
		const auto status = git_repository_head(&ref, m_repo);
		ensureOk(status);

		return ref;
	}

	auto head(const std::string& worktree)
	{
		git_reference* ref = nullptr;
		const auto status = git_repository_head_for_worktree(&ref, m_repo, worktree.c_str());
		ensureOk(status);

		return ref;
	}

	auto setHead(const std::string& refname)
	{
		const auto status = git_repository_set_head(m_repo, refname.c_str());
		ensureOk(status);
	}

	auto setHeadDetached(const Oid& committish)
	{
		const auto status = git_repository_set_head_detached(m_repo, &committish);
		ensureOk(status);
	}

	// TODO: setHeadDetached(annotated commit)

	auto isHeadDetached()
	{
		const auto result = git_repository_head_detached(m_repo);
		ensureOk(result);

		return static_cast<bool>(result);
	}

	auto isHeadDetached(const std::string& worktree)
	{
		const auto result = git_repository_head_detached_for_worktree(m_repo, worktree.c_str());
		ensureOk(result);

		return static_cast<bool>(result);
	}

	auto isHeadUnborn()
	{
		const auto result = git_repository_head_unborn(m_repo);
		ensureOk(result);

		return static_cast<bool>(result);
	}

	auto ident() const
	{
		const char* name = nullptr;
		const char* email = nullptr;
		const auto status = git_repository_ident(&name, &email, m_repo);
		ensureOk(status);

		return std::pair { std::string_view { name }, std::string_view { email } };
	}

	auto setIdent(const std::string& name, const std::string& email)
	{
		const auto status = git_repository_set_ident(m_repo, name.c_str(), email.c_str());
		ensureOk(status);
	}

	// TODO: index (needs index)

	auto isBare() const
	{
		const auto result = git_repository_is_bare(m_repo);
		ensureOk(result);

		return static_cast<bool>(result);
	}

	auto isEmpty()
	{
		const auto result = git_repository_is_empty(m_repo);
		ensureOk(result);

		return static_cast<bool>(result);
	}

	auto isShallow()
	{
		const auto result = git_repository_is_shallow(m_repo);
		ensureOk(result);

		return static_cast<bool>(result);
	}

	auto isWorkTree() const
	{
		const auto result = git_repository_is_worktree(m_repo);
		ensureOk(result);

		return static_cast<bool>(result);
	}

	auto getItemPath(git_repository_item_t item) const
	{
		Buffer buf;
		const auto status = git_repository_item_path(&buf, m_repo, item);
		ensureOk(status);

		return buf;
	}

	// TODO: merge heads

	auto message()
	{
		Buffer buf;
		const auto status = git_repository_message(&buf, m_repo);
		ensureOk(status);

		return buf;
	}

	auto removeMessage()
	{
		const auto status = git_repository_message_remove(m_repo);
		ensureOk(status);
	}

	// TODO: odb (needs odb)

	auto path() const
	{
		const auto path = git_repository_path(m_repo);
		return std::string_view { path };
	}

	auto workdir() const
	{
		const auto result = git_repository_workdir(m_repo);
		return std::string_view { result };
	}

	auto setWorkdir(const std::string& workdir, bool update_gitlink = false)
	{
		const auto status = git_repository_set_workdir(m_repo, workdir.c_str(), update_gitlink);
		ensureOk(status);
	}

	// TODO: refdb (needs refdb)

	auto state()
	{
		const auto result = git_repository_state(m_repo);

		return static_cast<git_repository_state_t>(result);
	}

	auto cleanupState()
	{
		const auto status = git_repository_state_cleanup(m_repo);
		ensureOk(status);
	}

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

	// TODO: open (worktree)

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

	// TODO: wrap (odb)

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
