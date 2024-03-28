#include <iostream>

#include "luaIncludes.hpp"
#include "CMatrixCorrelator.hpp"
#include "MatrixCorrelatorLuaAdapter.hpp"
import CImageSegmenter;
#include "ImageSegmenterLuaAdapter.hpp"
#include "ImageUtilities.hpp"

void runTests(lua_State *L) {
    luaL_dofile(L, "runtests.lua");
}

void runAll(lua_State *L) {
    // Drivers
    luaL_dofile(L, "correlation_driver.lua");
    luaL_dofile(L, "segmentation_driver.lua");
}

lua_State* luaSetup(CMatrixCorrelator& correlator, CImageSegmenter& segmenter) {
    // Kick up Lua
    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);

    *static_cast<CMatrixCorrelator**>(lua_getextraspace(L)) = &correlator;
    *static_cast<CImageSegmenter**>(lua_getextraspace(L)) = &segmenter;

    // === Lua <-> Cpp function map
    // Matrix correlation
    lua_register(L, "toCppCorrelateMatrix", &matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaCorrelateMatrix>);
    lua_register(L, "toCppSetCorrelationModeBasic", &matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeBasic>);
    lua_register(L, "toCppSetCorrelationModeILP", &matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeILP>);
    lua_register(L, "toCppSetCorrelationModeParallel", &matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeParallel>);
    lua_register(L, "toCppSetCorrelationModeVectorised", &matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeVectorised>);
    lua_register(L, "toCppSetCorrelationModeMaxOptDPrec", &matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeMaxOptDPrec>);
    lua_register(L, "toCppSetCorrelationModeMaxOptSPrec", &matrixCorrelatorDispatch<&CMatrixCorrelator::fromLuaSetModeMaxOptSPrec>);
    
    // Image segmentation
    lua_register(L, "toCppSegmentImage", &imageSegmenterDispatch<&CImageSegmenter::fromLuaSegmentImage>);
    lua_register(L, "toCppSetSegmentationModeOptimised", &imageSegmenterDispatch<&CImageSegmenter::fromLuaSetModeOptimised>);

    // Utilities
    lua_register(L, "toCppReadRGBImageFromFile", fromLuaReadRGBImageFromFile);
    lua_register(L, "toCppWriteRGBImageToFile", fromLuaWriteRGBImageToFile);
    lua_register(L, "toCppCreateImageFromSegmentationDescription", fromLuaCreateImageFromSegmentationDescription);

    return L;
}

void luaTearDown(lua_State *L) {
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