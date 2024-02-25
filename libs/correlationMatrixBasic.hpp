#pragma once

#include "luaIncludes.hpp"

void correlate(int ny, int nx, const float *data, float *result);

extern "C" int luaCorrelate(lua_State *L);