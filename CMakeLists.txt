project(lab02-cv)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 3.30)

file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})

find_package(OpenGL REQUIRED)

set(SDL3_DIR "${CMAKE_SOURCE_DIR}/external/SDL3")
find_package(SDL3 REQUIRED CONFIG)

file(GLOB IMGUI_SOURCES "external/imgui/*.cpp")

add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC external/imgui)
target_link_libraries(imgui PRIVATE SDL3::SDL3)

add_library(glad "external/glad/src/glad.c")
target_include_directories(glad PUBLIC external/glad/include)

target_link_libraries(main PRIVATE SDL3::SDL3 OpenGL::GL imgui glad)
