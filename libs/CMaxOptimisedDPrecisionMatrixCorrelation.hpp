#pragma once

#include "IMatrixCorrelationStrategy.hpp"

class CMaxOptimisedDPrecisionMatrixCorrelation : public IMatrixCorrelationStrategy {
public:
    CMaxOptimisedDPrecisionMatrixCorrelation();
    void correlate(
        int ny,
        int nx,
        float *data,
        float *result
    ) override;   
};