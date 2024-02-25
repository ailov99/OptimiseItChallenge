#include <iostream>

#include "luaIncludes.hpp"
#include "correlationMatrixBasic.hpp"

int main(int argc, char **argv) {
    // Kick up Lua
    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);

    // Lua logic
    lua_register(L, "luaCorrelate", luaCorrelate);
    luaL_dofile(L, "correlation_driver.lua");

    // Cleanup
    lua_close(L);
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore();

    return 0;
}