#pragma once

class IMatrixCorrelationStrategy 
{
public:
    virtual ~IMatrixCorrelationStrategy() = default;
    virtual void correlate(
        int ny,
        int nx,
        float *data,
        float *result
    ) = 0;
};