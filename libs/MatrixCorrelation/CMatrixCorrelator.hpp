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
    int fromLuaSetModeParallel(lua_State *L);
    int fromLuaSetModeVectorised(lua_State *L);
    int fromLuaSetModeMaxOptDPrec(lua_State *L);
    int fromLuaSetModeMaxOptSPrec(lua_State *L);

private:
    void correlateMatrix(
        int ny,
        int nx,
        float *data,
        float *result
    );
    void setModeBasic();
    void setModeILP();
    void setModeParallel();
    void setModeVectorised();
    void setModeMaxOptDPrec();
    void setModeMaxOptSPrec();

    std::unique_ptr<IMatrixCorrelationStrategy> m_correlation_strat;

};