#include "COptimisedImageSegmentation.hpp"

COptimisedImageSegmentation::COptimisedImageSegmentation() {
}

SegmentationDescription COptimisedImageSegmentation::segment(
    int ny,
    int nx,
    const float *in_data
) {
    // TODO
    return SegmentationDescription{0,0,0,0,{0.f,0.f,0.f},{0.f,0.f,0.f}};
}