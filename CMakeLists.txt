cmake_minimum_required(VERSION 3.10)
project(windowing LANGUAGES CXX C VERSION 0.0.1)

set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/)
file(GLOB SHADERS "${SHADERS_DIR}/*.vert" "${SHADERS_DIR}/*.frag")

set(SRC src/main.cpp src/shader.hpp src/debug.hpp)
set(VENDOR src/glad.c)
set(ALL_SRC ${SRC} ${VENDOR} ${SHADERS})

source_group("Source" FILES ${SRC})
source_group("Vendor" FILES ${VENDOR})
source_group("Shaders" FILES ${SHADERS})

include_directories(include src)
add_subdirectory(vendor/glfw)
find_package(OpenGL REQUIRED)

add_executable(main ${ALL_SRC}) 
target_link_libraries(main PRIVATE glfw)
