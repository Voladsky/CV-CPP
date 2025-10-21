cmake_minimum_required(VERSION 3.30)
project(lab02-cv)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/framework/*.cpp")
file(GLOB_RECURSE HEADERS "include/framework/*.h")

file(GLOB_RECURSE LUMA_SOURCES "src/luma/*.cpp")
add_executable(main ${SOURCES} ${HEADERS} ${LUMA_SOURCES})

target_include_directories(main PRIVATE
    "include/framework"
    "include/luma"
)

find_package(OpenGL REQUIRED)

set(ENV{SDL3_DIR} "${CMAKE_SOURCE_DIR}/external/SDL3")
find_package(SDL3 REQUIRED CONFIG)
set(ENV{OpenCV_DIR} "${CMAKE_SOURCE_DIR}/external/opencv")
find_package(OpenCV REQUIRED)

file(GLOB IMGUI_SOURCES "external/imgui/*.cpp")

add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC external/imgui)
target_link_libraries(imgui PRIVATE SDL3::SDL3)


target_link_libraries(main PRIVATE SDL3::SDL3 OpenGL::GL imgui ${OpenCV_LIBS})

file(GLOB_RECURSE OTSU_SOURCES "src/otsu/*.cpp")
add_executable(otsu ${SOURCES} ${HEADERS} ${OTSU_SOURCES})
target_include_directories(otsu PRIVATE
    "include/framework"
    "include/otsu"
)
target_link_libraries(otsu PRIVATE SDL3::SDL3 OpenGL::GL imgui ${OpenCV_LIBS})
