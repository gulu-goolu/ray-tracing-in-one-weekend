cmake_minimum_required(VERSION 3.15)

project(ray-tracing-in-one-weekend)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3rdparty)

file(GLOB_RECURSE SOURCE_LIST src/*.h src/*.cpp)
add_executable(demo ${SOURCE_LIST})
target_link_libraries(demo 3rdparty_stub)