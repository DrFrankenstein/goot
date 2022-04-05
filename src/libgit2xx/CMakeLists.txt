add_library(git2xx INTERFACE)

target_sources(
	git2xx
	PRIVATE
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
