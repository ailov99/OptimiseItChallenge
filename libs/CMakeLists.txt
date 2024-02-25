set(HEADER_LIST "${OptimiseItChallenge_SOURCE_DIR}/include/luaIncludes.hpp")

# Make an automatic library - will be static or dynamic based on user setting
add_library(correlation correlationMatrixBasic.cpp ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(correlation PUBLIC ../include)

# Find and link Lua
find_package(Lua 5.4 REQUIRED)
target_include_directories(correlation PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(correlation ${LUA_LIBRARIES})

# All users of this library will need at least C++17
target_compile_features(correlation PUBLIC cxx_std_17)

# IDEs should put the headers in a nice place
source_group(
  TREE "${OptimiseItChallenge_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
)
