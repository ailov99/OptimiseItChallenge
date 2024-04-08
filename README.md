# OptimiseItChallenge
This is personal effort to optimise various algorithms as much as possible using various techniques (in C++, using Lua as glue). 

A lot of the library code was written as part of a Master's course in parallel programming that I took years ago. I am now looking to improve it and have it tested (while also learning Lua).

Logic is implemented under /libs while Lua drivers are in /src/lua_drivers. Glued together in /src/main.cpp.

# Dependencies:
- C++20 (or newer)
- Ninja generator
- CMake 3.29 (or newer)
- Lua 5.4 (or newer)
- OpenMP
- stb_image, stb_image_write
- luarocks install luasocket

**Note: This project is currently being developed on MacOS and hasn't been throughly tested on other platforms yet.**

# To run:
Launch ./run_app.sh with the following flags:
- -config
- -clean
- -build (Note: Always run this first)
- -run
- -test (runs all tests)