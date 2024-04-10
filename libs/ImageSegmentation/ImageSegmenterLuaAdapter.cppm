module;

#include "luaIncludes.hpp"
import CImageSegmenter;

export module ImageSegmenterLuaAdapter;

export namespace ImageSegmenterAdapter {
    typedef int (CImageSegmenter::*mem_func)(lua_State * L);

    // This template wraps a member function into a C-style "free" function compatible with lua.
    template <mem_func func>
    auto imageSegmenterDispatch(lua_State * L) -> int {
        auto* ptr = *static_cast<CImageSegmenter**>(lua_getextraspace(L));
        return ((*ptr).*func)(L);
    }
};
