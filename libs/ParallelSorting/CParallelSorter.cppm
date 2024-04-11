module;

#include <concepts>
#include <memory>
#include <iostream>
#include <algorithm>
#include <omp.h>
#include "luaIncludes.hpp"

export module CParallelSorter;

export template <typename T>
concept Sortable = requires (T a, T b)
{
    a <=> b; 
};

export class CParallelSorter
{
public:
    CParallelSorter();

    template <Sortable T>
    auto fromLuaMergeSort(lua_State *L) -> int;

private:
    auto mergeSort(
        Sortable auto *data,
        const int data_len
    ) -> void;

};

CParallelSorter::CParallelSorter() {
}

template <Sortable T>
auto CParallelSorter::fromLuaMergeSort(lua_State *L) -> int {
    // Get data length
    const auto data_len {lua_tonumber(L, 1)};

    // Get data (lua table)
    luaL_checktype(L, 2, LUA_TTABLE);
    lua_settop(L, 2);
    const auto table_len {lua_rawlen(L, 2)};
    std::vector<T> in_data(table_len);

    for (auto i = 0; i < table_len; i++) {
        lua_pushinteger(L, i+1);
        lua_gettable(L, -2);

        if (lua_isnumber(L, -1)) {
            in_data[i] = lua_tonumber(L, -1);
        } else {
            std::cout << "Failed to parse table (nan)" << std::endl;
            lua_error(L);
        }

        lua_pop(L,1);
    }

    // Sort
    mergeSort(in_data.data(), data_len);

    // Push sorted data back onto stack
    lua_createtable(L, table_len, 0);
    const auto ret_table {lua_gettop(L)};
    for (auto i = 0; i < table_len; i++) {
        lua_pushnumber(L, in_data[i]);
        lua_rawseti(L, ret_table, i+1);
    }

    return 1;
}

auto CParallelSorter::mergeSort(Sortable auto *data, const int data_len) -> void {
    auto n_threads {omp_get_max_threads()};
    auto pow_of_two {1};
    while (n_threads > pow_of_two)
        pow_of_two <<= 1;
    n_threads = pow_of_two >> 1;

    // Check if worth doing this multi-threaded
    if (n_threads == 1) {
        std::sort(data, data+data_len);
        return;
    }

    auto chunk_size_per_thread{(data_len+n_threads-1)/n_threads};

    // First, all threads sort their respective chunks
    #pragma omp parallel num_threads(n_threads) 
    {
        const auto this_thread {omp_get_thread_num()};
        // Fine to use std::sort here as it's each thread doing it in parallel
        const auto this_thread_chunk_start {std::clamp(this_thread * chunk_size_per_thread, 0, data_len)};
        const auto this_thread_chunk_end {std::clamp(this_thread_chunk_start + chunk_size_per_thread, 0, data_len)};
        std::sort(&data[this_thread_chunk_start], &data[this_thread_chunk_end]);
    }

    // Now halve threads and merge sorted chunks until no more work to do
    // also double the chunk size as we are processing twice more data per-thread
    for (
        auto merge_n_threads = n_threads >> 1; 
        merge_n_threads > 0; 
        merge_n_threads >>= 1, chunk_size_per_thread *= 2
    ) {
        #pragma omp parallel num_threads(merge_n_threads) 
        {
            const auto this_thread {omp_get_thread_num()};
            const auto this_thread_chunk_start {this_thread * 2};

            const auto first_chunk_start {std::clamp(this_thread_chunk_start * chunk_size_per_thread, 0, data_len)};
            const auto first_chunk_end_second_chunk_start {std::clamp(first_chunk_start + chunk_size_per_thread, 0, data_len)};
            const auto second_chunk_end {std::clamp(first_chunk_end_second_chunk_start + chunk_size_per_thread, 0, data_len)};

            std::inplace_merge(&data[first_chunk_start], &data[first_chunk_end_second_chunk_start], &data[second_chunk_end]);
        }
    }
}