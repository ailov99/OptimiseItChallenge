#include <vector>
#include <iostream>
#include "CImageSegmenter.hpp"
#include "COptimisedImageSegmentation.hpp"

CImageSegmenter::CImageSegmenter(){
}

int CImageSegmenter::fromLuaSetModeOptimised(lua_State *L) {
    setModeOptimised();
    return 0;
}

void CImageSegmenter::setModeOptimised() {
    m_segmentation_strat = std::make_unique<COptimisedImageSegmentation>();
}

int CImageSegmenter::fromLuaSegmentImage(lua_State *L) {
    // Get params from Lua stack
    const auto ny = lua_tonumber(L, 1);
    const auto nx = lua_tonumber(L, 2);
    
    // Handle input matrix (lua table)
    luaL_checktype(L, 3, LUA_TTABLE);
    lua_settop(L, 3);
    const auto table_len = lua_rawlen(L, 3);
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
    const auto segDescription = segment(ny,nx,in_matrix.data());

    // Push descriptor onto the stack
    lua_createtable(L, table_len, 0);
    int ret_table = lua_gettop(L);
    int ret_table_index = 1;

    lua_pushnumber(L, segDescription.y0);
    lua_rawseti(L, ret_table, ret_table_index++);
    lua_pushnumber(L, segDescription.x0);
    lua_rawseti(L, ret_table, ret_table_index++);
    lua_pushnumber(L, segDescription.y1);
    lua_rawseti(L, ret_table, ret_table_index++);
    lua_pushnumber(L, segDescription.x1);
    lua_rawseti(L, ret_table, ret_table_index++);
    for (int i = 0; i < 2; i++) {
        lua_pushnumber(L, segDescription.outer[i]);
        lua_rawseti(L, ret_table, ret_table_index++);
    }
    for (int i = 0; i < 2; i++) {
        lua_pushnumber(L, segDescription.inner[i]);
        lua_rawseti(L, ret_table, ret_table_index++);
    }

    return 1;
}

SegmentationDescription CImageSegmenter::segment(
    int ny,
    int nx,
    const float *in_data
) {
    return m_segmentation_strat->segment(
        ny, nx, in_data
    );
}