#include <iostream>

#include "luaIncludes.hpp"
import CMatrixCorrelator;
import MatrixCorrelatorLuaAdapter;
import CImageSegmenter;
import ImageSegmenterLuaAdapter;
import ImageUtilities;

auto runTests(lua_State *L) -> void {
    luaL_dofile(L, "runtests.lua");
}

auto runAll(lua_State *L) -> void {
    // Drivers
    luaL_dofile(L, "correlation_driver.lua");
    luaL_dofile(L, "segmentation_driver.lua");
}

auto luaSetup(CMatrixCorrelator& correlator, CImageSegmenter& segmenter) -> lua_State* {
    // Kick up Lua
    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);

    *static_cast<CMatrixCorrelator**>(lua_getextraspace(L)) = &correlator;
    *static_cast<CImageSegmenter**>(lua_getextraspace(L)) = &segmenter;

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
    auto *luaState = luaSetup(correlator, segmenter);

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