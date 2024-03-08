#pragma once

#include "IMatrixCorrelationStrategy.hpp"

class CMaxOptimisedSPrecisionMatrixCorrelation : public IMatrixCorrelationStrategy {
public:
    CMaxOptimisedSPrecisionMatrixCorrelation();
    void correlate(
        int ny,
        int nx,
        float *data,
        float *result
    ) override;   
};