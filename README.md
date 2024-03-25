# OptimiseItChallenge
This is personal effort to optimise various algorithms as much as possible using various techniques (in C++, using Lua as glue)

Logic is implemented under /libs while Lua drivers are in /src/lua_drivers. Glued together in /src/main.cpp.

# Dependencies:
- C++17 (or newer)
- Lua 5.4 (or newer)
- OpenMP
- stb_image, stb_image_write
- luarocks install luasocket

**Note: This project is currently being developed on MacOS and hasn't been throughly tested on other platforms yet.**

# To run:
Launch ./run_app.sh with the following flags:
- -c (config)
- -b (build)
- -r (run)
- -t (test)