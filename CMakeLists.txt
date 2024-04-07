cmake_minimum_required(VERSION 3.29)

project(
  OptimiseItChallenge
  VERSION 0.1
  DESCRIPTION "OptimiseItChallenge"
  LANGUAGES CXX
)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

endif()

set(CMAKE_CXX_STANDARD 20)

# FetchContent added in CMake 3.11, downloads during the configure step
# FetchContent_MakeAvailable was added in CMake 3.14; simpler usage
include(FetchContent)

# Executable drivers
add_subdirectory(src)

# Libraries
add_subdirectory(libs)

# Set platform
if(WIN32)
  set(IS_WIN TRUE)
elseif(APPLE)
  set(IS_MAC TRUE)
elseif(UNIX)
  set(IS_LINUX TRUE)
endif()
