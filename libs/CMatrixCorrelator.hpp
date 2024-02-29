#pragma once

#include <memory>
#include "IMatrixCorrelationStrategy.hpp"
#include "luaIncludes.hpp"

class CMatrixCorrelator
{
public:
    CMatrixCorrelator();
    int fromLuaCorrelateMatrix(lua_State *L);
    int fromLuaSetModeBasic(lua_State *L);
    int fromLuaSetModeILP(lua_State *L);

private:
    void correlateMatrix(
        int ny,
        int nx,
        float *data,
        float *result
    );
    void setModeBasic();
    void setModeILP();

    std::unique_ptr<IMatrixCorrelationStrategy> m_correlation_strat;

};