module;

export module IMatrixCorrelationStrategy;

export class IMatrixCorrelationStrategy 
{
public:
    virtual ~IMatrixCorrelationStrategy() = default;
    virtual auto correlate(
        const int ny,
        const int nx,
        const float* const data,
        float *result
    ) -> void = 0;
};