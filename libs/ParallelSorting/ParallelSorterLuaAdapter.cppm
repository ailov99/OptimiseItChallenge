module;

#include "luaIncludes.hpp"
import CParallelSorter;

export module ParallelSorterLuaAdapter;

export namespace ParallelSorterAdapter {
    typedef int (CParallelSorter::*mem_func)(lua_State * L);

    // This template wraps a member function into a C-style "free" function compatible with lua.
    template <mem_func func>
    auto parallelSorterDispatch(lua_State * L) -> int {
        auto* ptr = *static_cast<CParallelSorter**>(lua_getextraspace(L));
        return ((*ptr).*func)(L);
    }
}