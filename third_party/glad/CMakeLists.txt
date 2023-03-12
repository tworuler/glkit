cmake_minimum_required(VERSION 3.10)

project(glad)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(glad STATIC ${PROJECT_SOURCE_DIR}/src/glad.c)