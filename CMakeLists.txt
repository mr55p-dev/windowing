cmake_minimum_required(VERSION 3.10)
project(windowing LANGUAGES CXX C VERSION 0.0.1)

set(SRC src/main.cpp src/glad.c)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include src)
add_subdirectory(vendor/glfw)
find_package(OpenGL REQUIRED)

add_executable(main ${SRC})
target_link_libraries(main PRIVATE glfw)
