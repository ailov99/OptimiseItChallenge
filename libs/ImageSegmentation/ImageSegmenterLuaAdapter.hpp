#pragma once

import CImageSegmenter;

typedef int (CImageSegmenter::*mem_funcc)(lua_State * L);

// This template wraps a member function into a C-style "free" function compatible with lua.
template <mem_funcc func>
int imageSegmenterDispatch(lua_State * L) {
    CImageSegmenter * ptr = *static_cast<CImageSegmenter**>(lua_getextraspace(L));
    return ((*ptr).*func)(L);
}