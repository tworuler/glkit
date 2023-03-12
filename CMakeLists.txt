cmake_minimum_required(VERSION 3.10)

project(glkit)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/third_party)
include_directories(${PROJECT_SOURCE_DIR}/third_party/imgui)
include_directories(${PROJECT_SOURCE_DIR}/third_party/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party/glm)

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glfw)

file(GLOB IMGUI_SRCS ${PROJECT_SOURCE_DIR}/third_party/imgui/*.cpp)
list(APPEND IMGUI_SRCS
    ${PROJECT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl3.cpp)
add_library(imgui STATIC ${IMGUI_SRCS})

add_executable(glkit_app ${PROJECT_SOURCE_DIR}/glkit/main.cpp)
target_link_libraries(glkit_app imgui glfw glad)