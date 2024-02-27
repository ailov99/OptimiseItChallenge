#include <vector>
#include <iostream>

#include "correlationMatrixBasic.hpp"

/*
Generate a correlation matrix of the input data.

@in
- ny = number of rows
- nx = number of columns
- data = input matrix
Note: Element at row y and column x is at data[x + y*nx]

@out
- result = output correlation matrix
Note: Correlation between rows i and row j is in result[i + j*ny]
Note: Only locations where 0 <= j <= i < ny are present (avoid redundancy)
*/
void correlateBasic(
    const int ny, 
    const int nx, 
    const float *data, 
    float *result
) {
    std::vector<double> normalised_m(nx*ny);

    // Sum-norm to 0 and then to 1
    for (auto y = 0; y < ny; y++) {
        const auto *row_start = &(data[y*nx]);

        auto row_sum = 0.;
        for (auto x = 0; x < nx; x++)
            row_sum += row_start[x];
        const auto row_mean = row_sum/nx;

        auto row_sq_sum = 0.;
        for (auto x = 0; x < nx; x++)
            row_sq_sum += (row_start[x]-row_mean)*(row_start[x]-row_mean);
        const auto row_factor = std::sqrt(row_sq_sum);

        for (auto x = 0; x < nx; x++)
            normalised_m[x + y*nx] = (row_start[x] - row_mean) / row_factor;
    }
    
    // Multiply normalised matrix by its transpose
    for (int i = 0; i < ny; ++i) {
        for (int j = 0; j <= i; ++j) {
            auto coeff = 0.;
            for (int k = 0; k < nx; ++k)
                coeff += normalised_m[k + i*nx] * normalised_m[k + j*nx];
            result[i + j*ny] = coeff;
        }
    }
}

/*
Lua-callable wrapper
*/
int luaCorrelateBasic(lua_State *L) {
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
    correlateBasic(ny,nx,in_matrix.data(),out_matrix.data());

    // Push it back onto the stack
    lua_createtable(L, table_len, 0);
    int ret_table = lua_gettop(L);
    for (int i = 0; i < table_len; i++) {
        lua_pushnumber(L, out_matrix[i]);
        lua_rawseti(L, ret_table, i+1);
    }

    return 1;
}