cmake_minimum_required(VERSION 3.10)
project(windowing LANGUAGES CXX C VERSION 0.0.1)

set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/)
file(GLOB SHADERS "${SHADERS_DIR}/*.vert" "${SHADERS_DIR}/*.frag")

set(TEXTURES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/textures)
file(GLOB TEXTURES "${TEXTURES_DIR}/*.png")

set(SRC 
	src/main.cpp 
	src/debug.hpp
	src/shader.hpp 
	src/texture.hpp 
)
set(VENDOR src/glad.c)
set(ALL_SRC ${SRC} ${VENDOR} ${SHADERS} ${TEXTURES})

source_group("Source" FILES ${SRC})
source_group("Vendor" FILES ${VENDOR})
source_group("Shaders" FILES ${SHADERS})
source_group("Textures" FILES ${TEXTURES})

include_directories(include src)
add_subdirectory(vendor/glfw)
find_package(OpenGL REQUIRED)

add_executable(main ${ALL_SRC}) 
target_link_libraries(main PRIVATE glfw)
