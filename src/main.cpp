#include <iostream>

#include "luaIncludes.hpp"
#include "CMatrixCorrelator.hpp"
#include "MatrixCorrelatorLuaAdapter.hpp"
#include "CImageSegmenter.hpp"
#include "ImageSegmenterLuaAdapter.hpp"

int main(int argc, char **argv) {
    // Kick up Lua
    lua_State *L;
    L = luaL_newstate();
    luaL_openlibs(L);

    CMatrixCorrelator correlator;
    *static_cast<CMatrixCorrelator**>(lua_getextraspace(L)) = &correlator;
    CImageSegmenter segmenter;
    *static_cast<CImageSegmenter**>(lua_getextraspace(L)) = &segmenter;

    // === Lua logic
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

    // Drivers
    luaL_dofile(L, "correlation_driver.lua");
    //luaL_dofile(L, "segmentation_driver.lua");

    // Cleanup
    lua_close(L);
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore();

    return 0;
}