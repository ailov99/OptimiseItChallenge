#pragma once

#include "IMatrixCorrelationStrategy.hpp"

class CBasicMatrixCorrelation : public IMatrixCorrelationStrategy {
public:
    CBasicMatrixCorrelation();
    void correlate(
        int ny,
        int nx,
        float *data,
        float *result
    ) override;
};