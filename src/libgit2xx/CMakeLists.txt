add_library(git2xx INTERFACE)

target_sources(
	git2xx
	PRIVATE
		git2xx.hpp

		git2xx/Git.hpp

		git2xx/Buffer.hpp
		git2xx/Error.hpp
		git2xx/Options.hpp
		git2xx/StrArray.hpp

		git2xx/Repository.hpp
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
