#pragma once

#include "luaIncludes.hpp"

void correlateILP(int ny, int nx, const float *data, float *result);

extern "C" int luaCorrelateILP(lua_State *L);