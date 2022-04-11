add_library(git2xx INTERFACE)

target_sources(
	git2xx
	PRIVATE
		Git.hpp

		Buffer.hpp
		Error.hpp
		StrArray.hpp

		Repository.hpp
 )

target_include_directories(
	git2xx
	INTERFACE
		.
)

target_link_libraries(
	git2xx
	INTERFACE
		libgit2::libgit2
)
