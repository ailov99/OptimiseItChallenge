set(HEADER_LIST 
  "${OptimiseItChallenge_SOURCE_DIR}/include/luaIncludes.hpp"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(correlation ${HEADER_LIST})

target_sources(
  correlation 
  PUBLIC 
  FILE_SET cxx_modules 
  TYPE CXX_MODULES 
  FILES 
    MatrixCorrelatorLuaAdapter.cppm
    IMatrixCorrelationStrategy.cppm
    CMatrixCorrelator.cppm
    CBasicMatrixCorrelation.cppm
    CILPMatrixCorrelation.cppm
    CParallelMatrixCorrelation.cppm
    CVectorisedMatrixCorrelation.cppm
    CMaxOptimisedDPrecisionMatrixCorrelation.cppm
    CMaxOptimisedSPrecisionMatrixCorrelation.cppm
)

# We need this directory, and users of our library will need it too
target_include_directories(correlation PUBLIC ../../include)

# Find and link Lua
find_package(Lua 5.4 REQUIRED)
target_include_directories(correlation PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(correlation ${LUA_LIBRARIES})

# Find and link OpenMP
# Note: On a Mac, the default Xcode clang does not support OMP, so we need the logic below
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
find_package(OpenMP)

# All users of this library will need at least C++20
target_compile_features(correlation PUBLIC cxx_std_20)
