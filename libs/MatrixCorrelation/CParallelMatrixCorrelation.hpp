#pragma once

#include "IMatrixCorrelationStrategy.hpp"

class CParallelMatrixCorrelation : public IMatrixCorrelationStrategy {
public:
    CParallelMatrixCorrelation();
    void correlate(
        int ny,
        int nx,
        float *data,
        float *result
    ) override;    
};