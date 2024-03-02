#include <vector>
#include <iostream>
#include "CMatrixCorrelator.hpp"
#include "CBasicMatrixCorrelation.hpp"
#include "CILPMatrixCorrelation.hpp"
#include "CParallelMatrixCorrelation.hpp"
#include "CVectorisedMatrixCorrelation.hpp"
#include "CMaxOptimisedDPrecisionMatrixCorrelation.hpp"

CMatrixCorrelator::CMatrixCorrelator() {
    m_correlation_strat = std::make_unique<CBasicMatrixCorrelation>();
}

int CMatrixCorrelator::fromLuaSetModeBasic(lua_State *L) {
    setModeBasic();
    return 0;
}

int CMatrixCorrelator::fromLuaSetModeILP(lua_State *L) {
    setModeILP();
    return 0;
}

int CMatrixCorrelator::fromLuaSetModeParallel(lua_State *L) {
    setModeParallel();
    return 0;
}

int CMatrixCorrelator::fromLuaSetModeVectorised(lua_State *L) {
    setModeVectorised();
    return 0;
}

int CMatrixCorrelator::fromLuaSetModeMaxOptDPrec(lua_State *L) {
    setModeMaxOptDPrec();
    return 0;
}

int CMatrixCorrelator::fromLuaCorrelateMatrix(lua_State *L) {
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

    // Correlate
    std::vector<float> out_matrix(table_len);
    correlateMatrix(ny,nx,in_matrix.data(),out_matrix.data());

    // Push it back onto the stack
    lua_createtable(L, table_len, 0);
    int ret_table = lua_gettop(L);
    for (int i = 0; i < table_len; i++) {
        lua_pushnumber(L, out_matrix[i]);
        lua_rawseti(L, ret_table, i+1);
    }

    return 1;
}

void CMatrixCorrelator::setModeBasic() {
    m_correlation_strat = std::make_unique<CBasicMatrixCorrelation>();
}

void CMatrixCorrelator::setModeILP() {
    m_correlation_strat = std::make_unique<CILPMatrixCorrelation>();
}

void CMatrixCorrelator::setModeParallel() {
    m_correlation_strat = std::make_unique<CParallelMatrixCorrelation>();
}

void CMatrixCorrelator::setModeVectorised() {
    m_correlation_strat = std::make_unique<CVectorisedMatrixCorrelation>();
}

void CMatrixCorrelator::setModeMaxOptDPrec() {
    m_correlation_strat = std::make_unique<CMaxOptimisedDPrecisionMatrixCorrelation>();
}

void CMatrixCorrelator::correlateMatrix(
    int ny,
    int nx,
    float *data,
    float *result
) {
    m_correlation_strat->correlate(
        ny, nx, data, result
    );
}