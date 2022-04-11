#pragma once

#include <git2/errors.h>

namespace Git
{
class Error
{
	public:
	// we have to accept int, as that's what libgit2 functions return
	explicit Error(int errorcode):
	    m_errorcode { static_cast<git_error_code>(errorcode) },
	    m_error { git_error_last() }
	{ }

	int errorcode() const noexcept
	{
		return m_errorcode;
	}

	const char* what() const noexcept
	{
		if (m_error == nullptr)
			return nullptr;

		return m_error->message;
	}

	private:
	git_error_code m_errorcode;
	const git_error* m_error;
};

inline auto ensureOk(int status)
{
	if (status < 0)
		throw Error { status };
}
}
