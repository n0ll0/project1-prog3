cmake_minimum_required(VERSION 3.10.0)

# Explicitly set the MinGW-w64 compilers (adjust paths if your toolchain is installed elsewhere)
# set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe" CACHE STRING "" FORCE)
# set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe" CACHE STRING "" FORCE)

project(coursework1 VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB PROJECT_DEPENDENCY
	"${CMAKE_CURRENT_SOURCE_DIR}/deps/*"
)

# Collect all sources in src/
file(GLOB_RECURSE PROJECT_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

file(GLOB PROJECT_LIBS
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.a"
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.dll"
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.so"
)

file(GLOB PROJECT_LIBS_HEADER
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.a"
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.lib"
)

file(GLOB PROJECT_LIBS_DLL
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.dll"
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.so"
)

add_executable(main ${PROJECT_SOURCES})

# Expose src/ headers to the target
target_include_directories(main PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Headers")

# Link the provided import library files found in libs/ by full path so the linker receives them.
# If these are MSVC-format import libraries and linking still fails, see the notes
# provided to convert/create a MinGW import library from the DLL.
## Prefer MinGW import library (libDataSource.a) when available. Otherwise fall back
## to the provided DataSource.lib. This avoids MSVC-import-lib vs MinGW linker format
## issues when using the ucrt64/mingw toolchain.

target_link_libraries(main PRIVATE ${PROJECT_LIBS})

# Place the produced executable directly in the build directory for easy running.
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# Automatically copy DLLs from libs/ to the build output directory after build
foreach(_lib IN LISTS PROJECT_LIBS_DLL)
	add_custom_command(TARGET main POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${_lib}"
			"$<TARGET_FILE_DIR:main>"
	)
endforeach()

# Copy all dependency files into the target runtime directory after build
foreach(_dep IN LISTS PROJECT_DEPENDENCY)
	add_custom_command(TARGET main POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${_dep}"
			"$<TARGET_FILE_DIR:main>"
	)
endforeach()
