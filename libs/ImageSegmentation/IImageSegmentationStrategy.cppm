module;

export module IImageSegmentationStrategy;

export struct SegmentationDescription {
    int y0;
    int x0;
    int y1;
    int x1;
    float outer[3];
    float inner[3];
};

export class IImageSegmentationStrategy
{
public:
    virtual ~IImageSegmentationStrategy() = default;
    virtual auto segment(
        int ny,
        int nx,
        const float *in_data
    ) -> SegmentationDescription = 0;
};