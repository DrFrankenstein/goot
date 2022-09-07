#pragma once

#include <git2/global.h>

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
};
}
