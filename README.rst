====
goot
====

All of git, but in a GUI.

Building
========

If you haven't yet, install a C++ compiler.
	Most compilers should work, just make sure that you have the latest
	version, or at least one that supports C++20 well.

	- On Windows: `Build Tools for Visual Studio 2022 <https://visualstudio.microsoft.com/fr/downloads/#build-tools-for-visual-studio-2022>`_.
	- On macOS: run :code:`xcode-select install`.
	- On Linux: This depends on your distribution, but you'll want a package
	  like 'g++-12' or 'clang-11'.

If you haven't yet, `install conan <https://conan.io/downloads.html>`_.
	Conan is a package manager that automatically downloads all the necessary
	dependencies for this project and puts them in a central location, ready
	to be found and used.

If you haven't yet, `install cmake <https://cmake.org/download/>`_.
	CMake is a tool that takes the project files, checks your environment, and
	turns all that into build scripts that will work on your system.

Create a directory for your build, and go to it.
	e.g. :code:`mkdir out && cd out`. [#out]_

Run CMake.
	e.g. :code:`cmake ..`, or `follow these instructions <https://cmake.org/runningcmake/>`_.

Build.
	e.g. :code:`cmake --build .`, or run whatever build system you've
	configured it to use (:code:`make`, :code:`ninja`, :code:`msbuild`...)

.. [#out] :file:`.gitignore` is already configured to ignore :file:`out/`, so
	this is the recommended directory to use.
