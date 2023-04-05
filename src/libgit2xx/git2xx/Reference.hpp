#pragma once

#include "Repository.hpp"
#include "StrArray.hpp"

#include <cstddef>
#include <git2/refs.h>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>

namespace Git
{
class Git;

class Reference
{
	public:
	Reference(git_reference* ref):
	    m_ref { ref }
	{ }

	~Reference()
	{
		git_reference_free(m_ref);
	}

	Reference(const Reference& other)
	{
		const auto status = git_reference_dup(&m_ref, other.m_ref);
		ensureOk(status);
	}

	auto operator=(const Reference& other) -> Reference&
	{
		git_reference_free(m_ref);

		const auto status = git_reference_dup(&m_ref, other.m_ref);
		ensureOk(status);

		return *this;
	}

	Reference(Reference&& other) noexcept:
	    m_ref { std::exchange(other.m_ref, nullptr) }
	{ }

	auto operator=(Reference&& other) noexcept -> Reference&
	{
		std::swap(m_ref, other.m_ref);
		return *this;
	}

	auto operator<=>(const Reference& other) const noexcept
	{
		return git_reference_cmp(m_ref, other.m_ref);
	}

	auto operator==(const Reference& other) const noexcept
	{
		return *this <=> other == 0;
	}

	operator bool() noexcept
	{
		return m_ref != nullptr;
	}

	auto target() const
	{
		return static_cast<const Oid*>(git_reference_target(m_ref));
	}

	auto peelTarget() const
	{
		return static_cast<const Oid*>(git_reference_target_peel(m_ref));
	}

	auto symbolicTarget() const
	{
		return std::string_view { git_reference_symbolic_target(m_ref) };
	}

	auto type() const
	{
		return git_reference_type(m_ref);
	}

	auto name() const
	{
		return std::string_view { git_reference_name(m_ref) };
	}

	auto resolve() const
	{
		git_reference* ptr = nullptr;
		const auto status = git_reference_resolve(&ptr, m_ref);

		Reference ref { ptr };
		ensureOk(status);

		return ref;
	}

	auto owner() const
	{  // FIXME: this does not return a Repository object because it owns the
		// underlying reference. There is probably a better way to wrap
		// git_repository* pointers...
		return git_reference_owner(m_ref);
	}

	auto symbolicSetTarget(const std::string& target, const std::string& log_message = {})
	{
		git_reference* ptr = nullptr;
		const auto status = git_reference_symbolic_set_target(&ptr, m_ref, target.c_str(), log_message.c_str());

		Reference ref { ptr };
		ensureOk(status);

		return ref;
	}

	auto setTarget(const Oid& id, const std::string& log_message = {})
	{
		git_reference* ptr = nullptr;
		const auto status = git_reference_set_target(&ptr, m_ref, &id, log_message.c_str());

		Reference ref { ptr };
		ensureOk(status);

		return ref;
	}

	auto rename(const std::string& new_name, bool force = false, const std::string& log_message = {})
	{
		git_reference* ptr = nullptr;
		const auto status = git_reference_rename(&ptr, m_ref, new_name.c_str(), force, log_message.c_str());

		Reference ref { ptr };
		ensureOk(status);

		return ref;
	}

	auto remove()
	{
		const auto status = git_reference_delete(m_ref);
		ensureOk(status);
	}

	static auto lookup(Repository& repo, const std::string& name)
	{
		git_reference* ptr = nullptr;
		const auto status = git_reference_lookup(&ptr, repo, name.c_str());

		Reference ref { ptr };
		ensureOk(status);

		return ref;
	}

	static auto nameToId(Repository& repo, const std::string& name)
	{
		Oid result;
		const auto status = git_reference_name_to_id(&result, repo, name.c_str());
		ensureOk(status);

		return result;
	}

	static auto dwim(Repository& repo, const std::string& shorthand)
	{
		git_reference* ptr = nullptr;
		const auto status = git_reference_dwim(&ptr, repo, shorthand.c_str());

		Reference ref { ptr };
		ensureOk(status);

		return ref;
	}

	static auto createSymbolicMatching(Repository& repo, const std::string& name, const std::string& target, const std::string& current_value, bool force = false, const std::string& log_message = {})
	{
		git_reference* ptr = nullptr;
		const auto status = git_reference_symbolic_create_matching(&ptr, repo, name.c_str(), target.c_str(), force, current_value.c_str(), log_message.c_str());

		Reference ref { ptr };

		if (status == GIT_EMODIFIED)
			return ref;

		ensureOk(status);

		return ref;
	}

	static auto createSymbolic(Repository& repo, const std::string& name, const std::string& target, bool force = false, const std::string& log_message = {})
	{
		git_reference* ptr = nullptr;
		const auto status = git_reference_symbolic_create(&ptr, repo, name.c_str(), target.c_str(), force, log_message.c_str());

		Reference ref { ptr };
		ensureOk(status);

		return ref;
	}

	static auto create(Repository& repo, const std::string& name, const Oid& oid, bool force = false, const std::string& log_message = {})
	{
		git_reference* ptr = nullptr;
		const auto status = git_reference_create(&ptr, repo, name.c_str(), &oid, force, log_message.c_str());

		Reference ref { ptr };
		ensureOk(status);

		return ref;
	}

	static auto createMatching(Repository& repo, const std::string& name, const Oid& oid, const Oid& current_id, bool force = false, const std::string& log_message = {})
	{
		git_reference* ptr = nullptr;
		const auto status = git_reference_create_matching(&ptr, repo, name.c_str(), &oid, force, &current_id, log_message.c_str());

		Reference ref { ptr };

		if (status == GIT_EMODIFIED)
			return ref;

		ensureOk(status);

		return ref;
	}

	static auto remove(Repository& repo, const std::string& name)
	{
		const auto status = git_reference_remove(repo, name.c_str());
		ensureOk(status);
	}

	static auto list(Repository& repo)
	{
		StrArray result;
		const auto status = git_reference_list(&result, repo);
		ensureOk(status);
	}

	// TODO: git_reference_foreach?
	// TODO: git_reference_foreach_name?

	private:
	git_reference* m_ref;
};

// The following iterators are InputIterator-ish: they're close enough, but they
// are not copyable, because git_reference_iterator cannot be copied, either.

// TODO: AbstractReferenceIterator and then add ReferenceNameIterator

class ReferenceIterator
{
	public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = Reference;
	// using pointer = Reference*;
	// using reference = Reference&;

	ReferenceIterator() { }

	ReferenceIterator(Repository& repo)
	{
		const auto status = git_reference_iterator_new(&m_iter, repo);
		ensureOk(status);

		consume();
	}

	ReferenceIterator(Repository& repo, const std::string& glob)
	{
		const auto status = git_reference_iterator_glob_new(&m_iter, repo, glob.c_str());
		ensureOk(status);

		consume();
	}

	~ReferenceIterator()
	{
		git_reference_iterator_free(m_iter);
	}

	auto operator*() const
	{
		return Reference { m_curr };
	}

	auto& operator++()
	{
		consume();
		return *this;
	}

	auto operator++(int)
	{
		// InputIterator does not seem to require it++ to return anything; which
		// is good for us, because we can't clone git_reference_iterator's.
		// This might reveal some weird edge-case bugs in some algorithms, but
		// this is what we're working with.
		consume();
	}

	auto operator==(const ReferenceIterator& other)
	{
		return m_curr == other.m_curr;
	}

	private:
	auto consume() -> bool
	{
		const auto status = git_reference_next(&m_curr, m_iter);

		if (status == GIT_ITEROVER)
		{
			m_curr = nullptr;
			return false;
		}

		ensureOk(status);

		return true;
	}

	git_reference_iterator* m_iter = nullptr;
	git_reference* m_curr = nullptr;
};

}