#pragma once

#include "CMatrixCorrelator.hpp"

typedef int (CMatrixCorrelator::*mem_func)(lua_State * L);

// This template wraps a member function into a C-style "free" function compatible with lua.
template <mem_func func>
int dispatch(lua_State * L) {
    CMatrixCorrelator * ptr = *static_cast<CMatrixCorrelator**>(lua_getextraspace(L));
    return ((*ptr).*func)(L);
}