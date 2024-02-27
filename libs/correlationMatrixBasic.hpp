#pragma once

#include "luaIncludes.hpp"

void correlateBasic(int ny, int nx, const float *data, float *result);

extern "C" int luaCorrelateBasic(lua_State *L);