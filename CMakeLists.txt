cmake_minimum_required(VERSION 3.21.3)

add_subdirectory(Vendor/glad)
add_subdirectory(Vendor/glfw)
add_subdirectory(Vendor/glm)
add_subdirectory(Vendor/stb_image)
add_subdirectory(Vendor/assimp)

project(OpenGL-Project-3D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/Libs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/Archives)

file(GLOB_RECURSE Sources
	Source/**.cpp
	Source/**.h
)

add_executable(OpenGL-Project-3D ${Sources})

target_include_directories(OpenGL-Project-3D PUBLIC
	Source
)

target_link_libraries(OpenGL-Project-3D PUBLIC
	glad
	glfw
	glm
	stb_image
	assimp
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	target_compile_definitions(OpenGL-Project-3D PUBLIC
		DEBUG
	)
endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
	target_compile_definitions(OpenGL-Project-3D PUBLIC
		NDEBUG
	)
endif ()