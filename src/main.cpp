#include <iostream>

#include "luaIncludes.hpp"
#include "CMatrixCorrelator.hpp"
#include "MatrixCorrelatorLuaAdapter.hpp"

int main(int argc, char **argv) {
    // Kick up Lua
    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);

    CMatrixCorrelator correlator;
    *static_cast<CMatrixCorrelator**>(lua_getextraspace(L)) = &correlator;

    // Lua logic
    lua_register(L, "toCppCorrelateMatrix", &dispatch<&CMatrixCorrelator::fromLuaCorrelateMatrix>);
    lua_register(L, "toCppSetCorrelationModeBasic", &dispatch<&CMatrixCorrelator::fromLuaSetModeBasic>);
    lua_register(L, "toCppSetCorrelationModeILP", &dispatch<&CMatrixCorrelator::fromLuaSetModeILP>);
    lua_register(L, "toCppSetCorrelationModeParallel", &dispatch<&CMatrixCorrelator::fromLuaSetModeParallel>);
    luaL_dofile(L, "correlation_driver.lua");

    // Cleanup
    lua_close(L);
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore();

    return 0;
}