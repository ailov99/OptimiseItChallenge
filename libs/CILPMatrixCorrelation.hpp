#pragma once

#include "IMatrixCorrelationStrategy.hpp"

class CILPMatrixCorrelation : public IMatrixCorrelationStrategy {
public:
    CILPMatrixCorrelation();
    void correlate(
        int ny,
        int nx,
        float *data,
        float *result
    ) override;
};