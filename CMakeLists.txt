cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "g++")
# set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

project(test LANGUAGES C CXX VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(includes)

file(GLOB SOURCES_SRC "src/*.cpp")
file(GLOB SOURCES_GAME "src/**/*.cpp")
list(APPEND SOURCES ${SOURCES_SRC})
list(APPEND SOURCES ${SOURCES_GAME})

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/world
        ${CMAKE_SOURCE_DIR}/src/projectiles
        ${CMAKE_SOURCE_DIR}/src/shapes
        ${CMAKE_SOURCE_DIR}/src/ui
    PUBLIC
        ${OPENGL_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/includes
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/utils
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenGL_gl_LIBRARY} glfw glad)
