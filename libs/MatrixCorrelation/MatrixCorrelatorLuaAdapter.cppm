module;

#include "luaIncludes.hpp"
import CMatrixCorrelator;

export module MatrixCorrelatorLuaAdapter;

export namespace MatrixCorrelatorAdapter {
    typedef int (CMatrixCorrelator::*mem_func)(lua_State * L);

    // This template wraps a member function into a C-style "free" function compatible with lua.
    template <mem_func func>
    auto matrixCorrelatorDispatch(lua_State * L) -> int {
        auto* ptr = *static_cast<CMatrixCorrelator**>(lua_getextraspace(L));
        return ((*ptr).*func)(L);
    }
}