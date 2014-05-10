# CMake file for wibble

cmake_minimum_required(VERSION 2.8)

project(WIBBLE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "out")

# Build an executable from the sources and headers using c++11
list( APPEND CMAKE_CXX_FLAGS "-std=c++11")
include_directories(include)
add_executable(wibble
  src/main.cpp
  src/glfw_utils.cpp include/glfw_utils.hpp 
)

# GLFW dependency. This assumes the GLFW source tree has been checked
# out into ./libs/glfw.
target_link_libraries(wibble glfw ${GLFW_LIBRARIES})
add_subdirectory(${WIBBLE_SOURCE_DIR}/libs/glfw)
include_directories(${WIBBLE_SOURCE_DIR}/libs/glfw/include)
