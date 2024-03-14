#pragma once

#include <memory>
#include "IImageSegmentationStrategy.hpp"
#include "luaIncludes.hpp"

class CImageSegmenter
{
public:
    CImageSegmenter();
    int fromLuaSegmentImage(lua_State *L);
    int fromLuaSetModeOptimised(lua_State *L);

private:
    SegmentationDescription segment(
        int ny,
        int nx,
        const float *in_data
    );
    void setModeOptimised();

    std::unique_ptr<IImageSegmentationStrategy> m_segmentation_strat;
};