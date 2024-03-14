#pragma once

struct SegmentationDescription {
    int y0;
    int x0;
    int y1;
    int x1;
    float outer[3];
    float inner[3];
};

class IImageSegmentationStrategy
{
public:
    virtual ~IImageSegmentationStrategy() = default;
    virtual SegmentationDescription segment(
        int ny,
        int nx,
        const float *in_data
    ) = 0;
};