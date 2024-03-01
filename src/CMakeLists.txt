set(HEADER_LIST 
    "${OptimiseItChallenge_SOURCE_DIR}/include/luaIncludes.hpp"
    "${OptimiseItChallenge_SOURCE_DIR}/libs/IMatrixCorrelationStrategy.hpp"
    "${OptimiseItChallenge_SOURCE_DIR}/libs/CMatrixCorrelator.hpp"
    "${OptimiseItChallenge_SOURCE_DIR}/libs/CBasicMatrixCorrelation.hpp" 
    "${OptimiseItChallenge_SOURCE_DIR}/libs/CILPMatrixCorrelation.hpp"
    "${OptimiseItChallenge_SOURCE_DIR}/libs/MatrixCorrelatorLuaAdapter.hpp"
    "${OptimiseItChallenge_SOURCE_DIR}/libs/CParallelMatrixCorrelation.hpp"
    "${OptimiseItChallenge_SOURCE_DIR}/libs/CVectorisedMatrixCorrelation.hpp"
)
set(SRC_LIST 
    "${OptimiseItChallenge_SOURCE_DIR}/src/main.cpp" 
    "${OptimiseItChallenge_SOURCE_DIR}/libs/CMatrixCorrelator.cpp" 
    "${OptimiseItChallenge_SOURCE_DIR}/libs/CBasicMatrixCorrelation.cpp"
    "${OptimiseItChallenge_SOURCE_DIR}/libs/CILPMatrixCorrelation.cpp"
    "${OptimiseItChallenge_SOURCE_DIR}/libs/CParallelMatrixCorrelation.cpp"
    "${OptimiseItChallenge_SOURCE_DIR}/libs/CVectorisedMatrixCorrelation.cpp"
)

add_executable(main ${SRC_LIST} ${HEADER_LIST})

target_include_directories(main PRIVATE ../libs)

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE correlation)
