cmake_minimum_required(VERSION 3.20)

project(TinyRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("./include")

file(GLOB_RECURSE SOURCES "./src/*.cpp")
file(GLOB_RECURSE INCLUDES "./include/*.h")

add_executable(TinyRenderer ${SOURCES} ${INCLUDES})

add_subdirectory(lib/glm)
target_link_libraries(TinyRenderer PUBLIC glm)