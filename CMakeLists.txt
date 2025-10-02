project(lab02-cv)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})

target_include_directories(main PRIVATE external/SDL3/include external/imgui)

target_link_directories(main PRIVATE external/SDL3/lib/x64)

target_link_libraries(main PRIVATE SDL3)
