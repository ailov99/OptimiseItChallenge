set(HEADER_LIST 
  "${OptimiseItChallenge_SOURCE_DIR}/include/luaIncludes.hpp"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(utilities ${HEADER_LIST})

target_sources(
  utilities
  PUBLIC
  FILE_SET cxx_modules
  TYPE CXX_MODULES
  FILES
    ImageUtilities.cppm
)

# We need this directory, and users of our library will need it too
target_include_directories(utilities PUBLIC ../../include)
# std_image
target_include_directories(utilities PUBLIC /usr/local/include)

# Find and link Lua
find_package(Lua 5.4 REQUIRED)
target_include_directories(utilities PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(utilities ${LUA_LIBRARIES})

# All users of this library will need at least C++20
target_compile_features(utilities PUBLIC cxx_std_20)