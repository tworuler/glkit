cmake_minimum_required(VERSION 3.10)

project(glkit)

set(CMAKE_CXX_STANDARD 11)
if (UNIX)
    add_compile_options(-Wall)
endif()

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/third_party)
include_directories(${PROJECT_SOURCE_DIR}/third_party/imgui)
include_directories(${PROJECT_SOURCE_DIR}/third_party/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party/glm)
include_directories(${PROJECT_SOURCE_DIR}/third_party/cppbase)

if (MSVC)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glad)
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glfw)

file(GLOB IMGUI_SRCS ${PROJECT_SOURCE_DIR}/third_party/imgui/*.cpp)
list(APPEND IMGUI_SRCS
    ${PROJECT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl3.cpp)
add_library(imgui STATIC ${IMGUI_SRCS})

set(LINK_LIBS imgui glfw)
if (MSVC)
    list(APPEND LINK_LIBS glad)
else()
    list(APPEND LINK_LIBS "-framework OpenGL")
endif()

add_executable(glkit_app ${PROJECT_SOURCE_DIR}/glkit/main.cpp)

target_link_libraries(glkit_app ${LINK_LIBS})