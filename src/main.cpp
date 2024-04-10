#include <iostream>

#include "luaIncludes.hpp"
import CMatrixCorrelator;
import MatrixCorrelatorLuaAdapter;
import CImageSegmenter;
import ImageSegmenterLuaAdapter;
import CParallelSorter;
import ParallelSorterLuaAdapter;
import ImageUtilities;

auto runTests(lua_State *L) -> void {
    luaL_dofile(L, "runtests.lua");
}

auto runAll(lua_State *L) -> void {
    // Drivers
    //luaL_dofile(L, "correlation_driver.lua");
    //luaL_dofile(L, "segmentation_driver.lua");
    luaL_dofile(L, "sorting_driver.lua");
}

auto luaSetup(CMatrixCorrelator& correlator, CImageSegmenter& segmenter, CParallelSorter& sorter) -> lua_State* {
    // Kick up Lua
    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);

    *static_cast<CMatrixCorrelator**>(lua_getextraspace(L)) = &correlator;
    *static_cast<CImageSegmenter**>(lua_getextraspace(L)) = &segmenter;
    *static_cast<CParallelSorter**>(lua_getextraspace(L)) = &sorter;

    // === Lua <-> Cpp function map
    // Matrix correlation
    lua_register(L, "toCppCorrelateMatrix", &MatrixCorrelatorAdapter::matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaCorrelateMatrix>);
    lua_register(L, "toCppSetCorrelationModeBasic", &MatrixCorrelatorAdapter::matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeBasic>);
    lua_register(L, "toCppSetCorrelationModeILP", &MatrixCorrelatorAdapter::matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeILP>);
    lua_register(L, "toCppSetCorrelationModeParallel", &MatrixCorrelatorAdapter::matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeParallel>);
    lua_register(L, "toCppSetCorrelationModeVectorised", &MatrixCorrelatorAdapter::matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeVectorised>);
    lua_register(L, "toCppSetCorrelationModeMaxOptDPrec", &MatrixCorrelatorAdapter::matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeMaxOptDPrec>);
    lua_register(L, "toCppSetCorrelationModeMaxOptSPrec", &MatrixCorrelatorAdapter::matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeMaxOptSPrec>);
    
    // Image segmentation
    lua_register(L, "toCppSegmentImage", &ImageSegmenterAdapter::imageSegmenterDispatch<&CImageSegmenter::fromLuaSegmentImage>);
    lua_register(L, "toCppSetSegmentationModeOptimised", &ImageSegmenterAdapter::imageSegmenterDispatch<&CImageSegmenter::fromLuaSetModeOptimised>);

    // Parallel sorting
    lua_register(L, "toCppMergeSortInt", &ParallelSorterAdapter::parallelSorterDispatch<&CParallelSorter::fromLuaMergeSort<int>>);
    lua_register(L, "toCppMergeSortDouble", &ParallelSorterAdapter::parallelSorterDispatch<&CParallelSorter::fromLuaMergeSort<double>>);

    // Utilities
    lua_register(L, "toCppReadRGBImageFromFile", fromLuaReadRGBImageFromFile);
    lua_register(L, "toCppWriteRGBImageToFile", fromLuaWriteRGBImageToFile);
    lua_register(L, "toCppCreateImageFromSegmentationDescription", fromLuaCreateImageFromSegmentationDescription);

    return L;
}

auto luaTearDown(lua_State *L) -> void {
    // Cleanup
    lua_close(L);
}

int main(int argc, char **argv) {
    CMatrixCorrelator correlator;
    CImageSegmenter segmenter;
    CParallelSorter sorter;
    auto *luaState = luaSetup(correlator, segmenter, sorter);

    const std::string cmd_op = argv[1];
    if (argc == 2) {
        if (cmd_op == "test")
            runTests(luaState);
        else if (cmd_op == "run")
            runAll(luaState);
    }
    
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore();

    luaTearDown(luaState);

    return 0;
}