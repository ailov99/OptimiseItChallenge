#pragma once

#include "IImageSegmentationStrategy.hpp"

class COptimisedImageSegmentation : public IImageSegmentationStrategy {
public:
    COptimisedImageSegmentation();
    SegmentationDescription segment(
        int ny,
        int nx,
        const float *in_data
    ) override;
};