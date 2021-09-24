cmake_minimum_required(VERSION 3.21.3)

project(glad)

set(Source
	src/glad.c
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE})

add_library(glad ${Source})

target_include_directories(glad PUBLIC
	include
)
