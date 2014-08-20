# CMake file for wibble. You may need to change some of the dependency
# lines at the bottom depending on where you have them installed.
#
# You will need the following:
#
#   o GLEW
#   o GLFW
#   o Eigen 3
#

cmake_minimum_required(VERSION 2.8)

project(WIBBLE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "out")


# Build an executable from the sources and headers using c++11
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
include_directories(include)
add_executable(wibble

  # Main
  src/main.cpp

  # glfw utils
  src/glfwutils/glfw_utils.cpp include/glfwutils/glfw_utils.hpp

  # stb image
  src/stbimage/stb_image.c include/stbimage/stb_image.h
  
  # utils
  include/utils/NonCopyable.hpp
  
  # Filesystem stuff
  src/filesystem/Path.cpp include/filesystem/Path.hpp

  # Graphics
  src/graphics/Texture.cpp include/graphics/Texture.hpp
  src/graphics/ShaderProgram.cpp include/graphics/ShaderProgram.hpp
  src/graphics/BufferObjects.cpp include/graphics/BufferObjects.hpp
  src/graphics/GraphicsSystem.cpp include/graphics/GraphicsSystem.hpp
  src/graphics/GraphicsObject.cpp include/graphics/GraphicsObject.hpp
)

# GLFW dependency. This assumes the GLFW source tree has been checked
# out into ./libs/glfw. I've done this because the ubuntu package for
# glfw seems to be out of date.
target_link_libraries(wibble glfw ${GLFW_LIBRARIES})
add_subdirectory(${WIBBLE_SOURCE_DIR}/libs/glfw)
include_directories(${WIBBLE_SOURCE_DIR}/libs/glfw/include)

# Eigen dependency. Assumes eigen has been installed to /usr/include.
include_directories(/usr/include/eigen3)

# CMake seems to find glew of its own accord.
find_package(GLEW REQUIRED)
target_link_libraries(wibble ${GLEW_LIBRARIES})
