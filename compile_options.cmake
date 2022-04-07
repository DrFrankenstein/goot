add_library(compile_options INTERFACE)

target_compile_features(compile_options INTERFACE cxx_std_20)

if(MSVC)
	# CMake supplies /W3 by default, and CL complains about both being specified
	string(REPLACE " /W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

	target_compile_options(
		compile_options
		INTERFACE
			/W4          # most warnings
			/w14242      # warn on lossy conversion
			/w14254      # warn on lossy bitfield conversion
			/w14265      # warn on non-virtual destructors on virtual classes
			/permissive- # pedantic: don't allow non-standard exts'
	)
else()
	# assuming clang or gcc compatible
	target_compile_options(
		compile_options
		INTERFACE
			-Wall
			-Wextra
			-pedantic
			-Wconversion
			-Wnon-virtual-dtor
			-Wunused
			-Woverloaded-virtual  # warn if overloading instead of overriding a virtual
	)
endif()
