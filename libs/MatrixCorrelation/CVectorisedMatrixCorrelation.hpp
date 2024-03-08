#pragma once

#include "IMatrixCorrelationStrategy.hpp"

class CVectorisedMatrixCorrelation : public IMatrixCorrelationStrategy {
public:
    CVectorisedMatrixCorrelation();
    void correlate(
        int ny,
        int nx,
        float *data,
        float *result
    ) override;    
};