set(HEADER_LIST 
  "${OptimiseItChallenge_SOURCE_DIR}/include/luaIncludes.hpp"
)
set(SRC_LIST 
  "${OptimiseItChallenge_SOURCE_DIR}/src/main.cpp" 
)

add_executable(main ${SRC_LIST} ${HEADER_LIST})



target_include_directories(main PRIVATE ../libs)
target_include_directories(main PRIVATE ../libs/MatrixCorrelation)
target_include_directories(main PRIVATE ../libs/ImageSegmentation)
target_include_directories(main PRIVATE ../libs/ParallelSorting)
target_include_directories(main PRIVATE ../libs/Utilities)

# Have at least c++20
target_compile_features(main PRIVATE cxx_std_20)

target_link_libraries(main PRIVATE correlation)
target_link_libraries(main PRIVATE segmentation)
target_link_libraries(main PRIVATE sorting)
target_link_libraries(main PRIVATE utilities)