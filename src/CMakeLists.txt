set(HEADER_LIST "${OptimiseItChallenge_SOURCE_DIR}/libs/correlationMatrixBasic.hpp")
set(SRC_LIST "${OptimiseItChallenge_SOURCE_DIR}/src/main.cpp" "${OptimiseItChallenge_SOURCE_DIR}/libs/correlationMatrixBasic.cpp")

add_executable(main ${SRC_LIST} ${HEADER_LIST})

target_include_directories(main PRIVATE ../libs)

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE correlation)
