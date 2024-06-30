cmake_minimum_required(VERSION 3.10)
project(lesson1)

set(CMAKE_CXX_STANDARD 11)

#file(COPY "D:\\Study\\GitHub\\tinyrenderer\\tgaimage.cpp" DESTINATION "../src")
#file(COPY "D:\\Study\\GitHub\\tinyrenderer\\tgaimage.h" DESTINATION "../include")

file(GLOB_RECURSE SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")

include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES})