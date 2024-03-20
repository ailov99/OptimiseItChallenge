set(HEADER_LIST 
  "${OptimiseItChallenge_SOURCE_DIR}/include/luaIncludes.hpp"
  "${OptimiseItChallenge_SOURCE_DIR}/include/stb_image.h"
  "${OptimiseItChallenge_SOURCE_DIR}/include/stb_image_write.h"
  "${OptimiseItChallenge_SOURCE_DIR}/libs/Utilities/ImageUtilities.hpp"
)
set(SRC_LIST 
  "${OptimiseItChallenge_SOURCE_DIR}/libs/Utilities/ImageUtilities.cpp"
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(utilities ${SRC_LIST} ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(utilities PUBLIC ../../include)

# Find and link Lua
find_package(Lua 5.4 REQUIRED)
target_include_directories(utilities PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(utilities ${LUA_LIBRARIES})

# All users of this library will need at least C++17
target_compile_features(utilities PUBLIC cxx_std_17)