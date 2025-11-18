cmake_minimum_required(VERSION 3.10.0)

# set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe" CACHE STRING "" FORCE)
# set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe" CACHE STRING "" FORCE)

project(coursework1 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(FATAL_ERROR "This project must be built as x64 when using MSVC because libs/DataSource.lib is 64-bit. Reconfigure with -A x64 or use an x64 Developer Command Prompt.")
endif()

file(GLOB PROJECT_DEPENDENCY
	"${CMAKE_CURRENT_SOURCE_DIR}/deps/*"
)

file(GLOB_RECURSE PROJECT_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)

file(GLOB PROJECT_LIBS_MSVC
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.lib"
)
file(GLOB PROJECT_LIBS_MINGW
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.a"
)

file(GLOB PROJECT_LIBS_DLL
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.dll"
	"${CMAKE_CURRENT_SOURCE_DIR}/libs/*.so"
)

add_executable(main ${PROJECT_SOURCES})

target_include_directories(main PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/Headers"
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)
	target_link_libraries(main PRIVATE ${PROJECT_LIBS_MSVC})
else()
	target_link_libraries(main PRIVATE ${PROJECT_LIBS_MINGW})
endif()

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

foreach(_lib IN LISTS PROJECT_LIBS_DLL)
	add_custom_command(TARGET main POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${_lib}"
			"$<TARGET_FILE_DIR:main>"
	)
endforeach()

foreach(_dep IN LISTS PROJECT_DEPENDENCY)
	add_custom_command(TARGET main POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${_dep}"
			"$<TARGET_FILE_DIR:main>"
	)
endforeach()

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/deps"
        "$<TARGET_FILE_DIR:main>"
)
