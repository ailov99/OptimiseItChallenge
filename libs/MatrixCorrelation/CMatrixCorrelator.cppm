module;

#include <memory>
#include "luaIncludes.hpp"
#include <vector>
#include <iostream>
import IMatrixCorrelationStrategy;
import CBasicMatrixCorrelation;
import CILPMatrixCorrelation;
import CParallelMatrixCorrelation;
import CVectorisedMatrixCorrelation;
import CMaxOptimisedDPrecisionMatrixCorrelation;
import CMaxOptimisedSPrecisionMatrixCorrelation;

export module CMatrixCorrelator;

export class CMatrixCorrelator
{
public:
    CMatrixCorrelator();
    auto fromLuaCorrelateMatrix(lua_State *L) -> int;
    auto fromLuaSetModeBasic(lua_State *L) -> int;
    auto fromLuaSetModeILP(lua_State *L) -> int;
    auto fromLuaSetModeParallel(lua_State *L) -> int;
    auto fromLuaSetModeVectorised(lua_State *L) -> int;
    auto fromLuaSetModeMaxOptDPrec(lua_State *L) -> int;
    auto fromLuaSetModeMaxOptSPrec(lua_State *L) -> int;

private:
    auto correlateMatrix(
        const int ny,
        const int nx,
        const float* const data,
        float *result
    ) -> void;
    auto setModeBasic() -> void;
    auto setModeILP() -> void;
    auto setModeParallel() -> void;
    auto setModeVectorised() -> void;
    auto setModeMaxOptDPrec() -> void;
    auto setModeMaxOptSPrec() -> void;

    std::unique_ptr<IMatrixCorrelationStrategy> m_correlation_strat;
};


CMatrixCorrelator::CMatrixCorrelator() {
    m_correlation_strat = std::make_unique<CBasicMatrixCorrelation>();
}

auto CMatrixCorrelator::fromLuaSetModeBasic(lua_State *L) -> int {
    setModeBasic();
    return 0;
}

auto CMatrixCorrelator::fromLuaSetModeILP(lua_State *L) -> int {
    setModeILP();
    return 0;
}

auto CMatrixCorrelator::fromLuaSetModeParallel(lua_State *L) -> int {
    setModeParallel();
    return 0;
}

auto CMatrixCorrelator::fromLuaSetModeVectorised(lua_State *L) -> int {
    setModeVectorised();
    return 0;
}

auto CMatrixCorrelator::fromLuaSetModeMaxOptDPrec(lua_State *L) -> int {
    setModeMaxOptDPrec();
    return 0;
}

auto CMatrixCorrelator::fromLuaSetModeMaxOptSPrec(lua_State *L) -> int {
    setModeMaxOptSPrec();
    return 0;
}

auto CMatrixCorrelator::fromLuaCorrelateMatrix(lua_State *L) -> int {
    // Get params from Lua stack
    const auto ny{lua_tonumber(L, 1)};
    const auto nx{lua_tonumber(L, 2)};
    
    // Handle input matrix (lua table)
    luaL_checktype(L, 3, LUA_TTABLE);
    lua_settop(L, 3);
    const auto table_len{lua_rawlen(L, 3)};
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
    const auto ret_table{lua_gettop(L)};
    for (auto i = 0; i < table_len; i++) {
        lua_pushnumber(L, out_matrix[i]);
        lua_rawseti(L, ret_table, i+1);
    }

    return 1;
}

auto CMatrixCorrelator::setModeBasic() -> void {
    m_correlation_strat = std::make_unique<CBasicMatrixCorrelation>();
}

auto CMatrixCorrelator::setModeILP() -> void {
    m_correlation_strat = std::make_unique<CILPMatrixCorrelation>();
}

auto CMatrixCorrelator::setModeParallel() -> void {
    m_correlation_strat = std::make_unique<CParallelMatrixCorrelation>();
}

auto CMatrixCorrelator::setModeVectorised() -> void {
    m_correlation_strat = std::make_unique<CVectorisedMatrixCorrelation>();
}

auto CMatrixCorrelator::setModeMaxOptDPrec() -> void {
    m_correlation_strat = std::make_unique<CMaxOptimisedDPrecisionMatrixCorrelation>();
}

auto CMatrixCorrelator::setModeMaxOptSPrec() -> void {
    m_correlation_strat = std::make_unique<CMaxOptimisedSPrecisionMatrixCorrelation>();
}

auto CMatrixCorrelator::correlateMatrix(
    const int ny,
    const int nx,
    const float* const data,
    float *result
) -> void {
    m_correlation_strat->correlate(
        ny, nx, data, result
    );
}