#pragma once

#include <string>
#include "luaIncludes.hpp"

void readRGBImageFromFile(const std::string &path, float *data);

extern "C" int fromLuaReadRGBImageFromFile(lua_State *L);

void writeRGBImageToFile(const std::string &path, const float *data);

extern "C" int fromLuaWriteRGBImageToFile(lua_State *L);