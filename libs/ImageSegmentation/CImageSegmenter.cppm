module;

#include <vector>
#include <iostream>
#include <memory>
#include "luaIncludes.hpp"

import IImageSegmentationStrategy;
import COptimisedImageSegmentation;

export module CImageSegmenter;

export class CImageSegmenter
{
public:
    CImageSegmenter();
    auto fromLuaSegmentImage(lua_State *L) -> int;
    auto fromLuaSetModeOptimised(lua_State *L) -> int;

private:
    auto segment(
        int ny,
        int nx,
        const float *in_data
    ) -> SegmentationDescription;
    auto setModeOptimised() -> void;

    std::unique_ptr<IImageSegmentationStrategy> m_segmentation_strat;
};


CImageSegmenter::CImageSegmenter(){
}

auto CImageSegmenter::fromLuaSetModeOptimised(lua_State *L) -> int {
    setModeOptimised();
    return 0;
}

auto CImageSegmenter::setModeOptimised() -> void {
    m_segmentation_strat = std::make_unique<COptimisedImageSegmentation>();
}

auto CImageSegmenter::fromLuaSegmentImage(lua_State *L) -> int {
    // Get params from Lua stack
    const auto ny{lua_tonumber(L, 1)};
    const auto nx{lua_tonumber(L, 2)};
    
    // Handle input matrix (lua table)
    luaL_checktype(L, 3, LUA_TTABLE);
    lua_settop(L, 3);
    const auto table_len{lua_rawlen(L, 3)};
    std::vector<float> in_matrix(table_len);

    for (auto i = 0; i < table_len; i++) {
        lua_pushinteger(L, i+1);
        lua_gettable(L, -2);

        if (lua_isnumber(L, -1)) {
            in_matrix[i] = lua_tonumber(L, -1);
        } else {
            std::cout << "Failed to parse table (nan)" << std::endl;
            lua_error(L);
        }

        lua_pop(L,1);
    }

    // Segment
    const auto segDescription{segment(ny,nx,in_matrix.data())};

    // Push descriptor onto the stack
    lua_newtable(L);
    const auto ret_table{lua_gettop(L)};
    auto ret_table_index{1};
    lua_pushnumber(L, segDescription.y0);
    lua_rawseti(L, ret_table, ret_table_index++);
    lua_pushnumber(L, segDescription.x0);
    lua_rawseti(L, ret_table, ret_table_index++);
    lua_pushnumber(L, segDescription.y1);
    lua_rawseti(L, ret_table, ret_table_index++);
    lua_pushnumber(L, segDescription.x1);
    lua_rawseti(L, ret_table, ret_table_index++);

    lua_newtable(L);
    const auto out_table{lua_gettop(L)};
    for (int i = 0; i < 3; i++) {
        lua_pushnumber(L, segDescription.outer[i]);
        lua_rawseti(L, out_table, i+1);
    }
    lua_rawseti(L, ret_table, ret_table_index++);

    lua_newtable(L);
    const auto in_table{lua_gettop(L)};
    for (int i = 0; i < 3; i++) {
        lua_pushnumber(L, segDescription.inner[i]);
        lua_rawseti(L, in_table, i+1);
    }
    lua_rawseti(L, ret_table, ret_table_index++);

    return 1;
}

auto CImageSegmenter::segment(
    int ny,
    int nx,
    const float *in_data
) -> SegmentationDescription {
    return m_segmentation_strat->segment(
        ny, nx, in_data
    );
}