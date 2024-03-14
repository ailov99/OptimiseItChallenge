#include "CImageSegmenter.hpp"
#include "COptimisedImageSegmentation.hpp"

CImageSegmenter::CImageSegmenter(){
}

int CImageSegmenter::fromLuaSetModeOptimised(lua_State *L) {
    setModeOptimised();
    return 0;
}

void CImageSegmenter::setModeOptimised() {
    m_segmentation_strat = std::make_unique<COptimisedImageSegmentation>();
}

int CImageSegmenter::fromLuaSegmentImage(lua_State *L) {
    // TODO
    return 0;
}

SegmentationDescription CImageSegmenter::segment(
    int ny,
    int nx,
    const float *in_data
) {
    return m_segmentation_strat->segment(
        ny, nx, in_data
    );
}