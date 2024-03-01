set(HEADER_LIST 
  "${OptimiseItChallenge_SOURCE_DIR}/include/luaIncludes.hpp"
)
set(SRC_LIST 
  "${OptimiseItChallenge_SOURCE_DIR}/libs/CMatrixCorrelator.cpp" 
  "${OptimiseItChallenge_SOURCE_DIR}/libs/CBasicMatrixCorrelation.cpp"
  "${OptimiseItChallenge_SOURCE_DIR}/libs/CILPMatrixCorrelation.cpp"
  "${OptimiseItChallenge_SOURCE_DIR}/libs/CParallelMatrixCorrelation.cpp"
  "${OptimiseItChallenge_SOURCE_DIR}/libs/CVectorisedMatrixCorrelation.cpp"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(correlation ${SRC_LIST} ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(correlation PUBLIC ../include)

# Find and link Lua
find_package(Lua 5.4 REQUIRED)
target_include_directories(correlation PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(correlation ${LUA_LIBRARIES})

# Find and link OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(correlation PUBLIC OpenMP::OpenMP_CXX)
endif()

# All users of this library will need at least C++17
target_compile_features(correlation PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
  TREE "${OptimiseItChallenge_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
)
