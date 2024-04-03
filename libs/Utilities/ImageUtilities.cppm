module;

#include <string>
#include <utility>
#include <iostream>
#include <vector>
#include "luaIncludes.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

export module ImageUtilities;

export auto readRGBImageFromFile(
    const std::string &path, 
    int &image_width,
    int &image_height,
    int &image_channels,
    unsigned char *&data
) -> void {
    int real_image_channels;
    data = stbi_load(path.c_str(), &image_width, &image_height, &real_image_channels, STBI_rgb);
    image_channels = STBI_rgb; // We get as many channels as requested
}

export extern "C" auto fromLuaReadRGBImageFromFile(lua_State *L) -> int {
    // Get params from Lua stack
    const char *file_path = lua_tostring(L, 1);
    std::string file_path_str(file_path);

    // Read image in
    int image_width, image_height, image_channels;
    unsigned char *callee_alloced_image_data;
    readRGBImageFromFile(
        file_path_str,
        image_width,
        image_height,
        image_channels,
        callee_alloced_image_data
    );

    // Push data onto lua stack
    lua_newtable(L);
    const auto ret_table{lua_gettop(L)};
    int ret_table_index{1};
    lua_pushnumber(L, image_width);
    lua_rawseti(L, ret_table, ret_table_index++);
    lua_pushnumber(L, image_height);
    lua_rawseti(L, ret_table, ret_table_index++);
    lua_pushnumber(L, image_channels);
    lua_rawseti(L, ret_table, ret_table_index++);

    lua_newtable(L);
    const auto data_table{lua_gettop(L)};
    const auto stack_expanded{lua_checkstack(L, image_width*image_height*image_channels)};

    for (
        auto all_components = image_width*image_height*image_channels, i = 0;
        i < all_components;
        i++
    ) {
        lua_pushnumber(L, callee_alloced_image_data[i]);
        lua_rawseti(L, data_table, i+1);
    }
    lua_rawseti(L, ret_table, ret_table_index++);
    
    stbi_image_free(callee_alloced_image_data);

    return 1;
}

export auto writeRGBImageToFile(
    const std::string &path, 
    const int image_width,
    const int image_height,
    const int image_channels,
    const unsigned char *data
) -> void {
    if(stbi_write_png(
        path.c_str(),
        image_width,
        image_height,
        image_channels,
        data,
        0
    ) != 1) {
        std::cout << "Failed to write image data to " << path << std::endl;
    }
}

export extern "C" auto fromLuaWriteRGBImageToFile(lua_State *L) -> int {
    // Get params from Lua stack
    const char *file_path{lua_tostring(L, 1)};
    std::string file_path_str(file_path);
    const auto image_width{lua_tonumber(L, 2)};
    const auto image_height{lua_tonumber(L, 3)};
    const auto image_channels{lua_tonumber(L, 4)};

    const auto stack_expanded{lua_checkstack(L, image_width*image_height*image_channels)};

    luaL_checktype(L, 5, LUA_TTABLE);
    lua_settop(L, 5);
    const auto table_len{lua_rawlen(L, 5)};
    std::vector<unsigned char> image_components(table_len);

    for (auto i = 0; i < table_len; i++) {
        lua_pushinteger(L, i+1);
        lua_gettable(L, -2);

        if (lua_isnumber(L, -1)) {
            image_components[i] = lua_tonumber(L, -1);
        } else {
            std::cout << "Failed to parse table (nan)" << std::endl;
            lua_error(L);
        }

        lua_pop(L,1);
    }

    writeRGBImageToFile(
        file_path_str,
        image_width,
        image_height,
        image_channels,
        image_components.data()
    );

    return 0;
}

export auto createImageFromSegmentationDescription(
    const std::pair<int,int> &seg_rect_y0x0,
    const std::pair<int,int> &seg_rect_y1x1,
    const std::vector<unsigned char> &seg_rect_rgb,
    const std::vector<unsigned char> &bg_rgb
) -> void {
    // TODO
}

export extern "C" auto fromLuaCreateImageFromSegmentationDescription(lua_State *L) -> int {
    // TODO
    return 0;
}