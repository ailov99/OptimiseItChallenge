set(HEADER_LIST 
  "${OptimiseItChallenge_SOURCE_DIR}/include/luaIncludes.hpp"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(segmentation ${HEADER_LIST})

target_sources(
  segmentation 
  PUBLIC 
  FILE_SET cxx_modules 
  TYPE CXX_MODULES 
  FILES 
    ImageSegmenterLuaAdapter.cppm
    CImageSegmenter.cppm
    COptimisedImageSegmentation.cppm
    IImageSegmentationStrategy.cppm
)

# We need this directory, and users of our library will need it too
target_include_directories(segmentation PUBLIC ../../include)

# Find and link Lua
find_package(Lua 5.4 REQUIRED)
target_include_directories(segmentation PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(segmentation ${LUA_LIBRARIES})

# Find and link OpenMP
# Note: On a Mac, the default Xcode clang does not support OMP, so we need the logic below
find_package(OpenMP)
if(APPLE)
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(OpenMP_C "${CMAKE_C_COMPILER}" CACHE STRING "" FORCE)
    set(OpenMP_C_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument" CACHE STRING "" FORCE)
    set(OpenMP_C_LIB_NAMES "libomp" "libgomp" "libiomp5" CACHE STRING "" FORCE)
    set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES} CACHE STRING "" FORCE)
    set(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES} CACHE STRING "" FORCE)
    set(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES} CACHE STRING "" FORCE)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(OpenMP_CXX "${CMAKE_CXX_COMPILER}" CACHE STRING "" FORCE)
  set(OpenMP_CXX_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument" CACHE STRING "" FORCE)
  set(OpenMP_CXX_LIB_NAMES "libomp" "libgomp" "libiomp5" CACHE STRING "" FORCE)
  set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES} CACHE STRING "" FORCE)
  set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES} CACHE STRING "" FORCE)
  set(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES} CACHE STRING "" FORCE)
endif()
endif()
target_link_libraries(segmentation ${OpenMP_CXX_FLAGS})

# All users of this library will need at least C++20
target_compile_features(segmentation PUBLIC cxx_std_20)