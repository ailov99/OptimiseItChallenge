#pragma once

#include <string>
#include <utility>
#include "luaIncludes.hpp"

void readRGBImageFromFile(
    const std::string &path, 
    int &image_width,
    int &image_height,
    int &image_channels,
    unsigned char *&data
);

extern "C" int fromLuaReadRGBImageFromFile(lua_State *L);

void writeRGBImageToFile(
    const std::string &path, 
    const int image_width,
    const int image_height,
    const int image_channels,
    const unsigned char *data
);

extern "C" int fromLuaWriteRGBImageToFile(lua_State *L);

void createImageFromSegmentationDescription(
    const std::pair<int,int> &seg_rect_y0x0,
    const std::pair<int,int> &seg_rect_y1x1,
    const std::vector<unsigned char> &seg_rect_rgb,
    const std::vector<unsigned char> &bg_rgb
);

extern "C" int fromLuaCreateImageFromSegmentationDescription(lua_State *L);