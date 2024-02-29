#include <vector>
#include "CBasicMatrixCorrelation.hpp"

CBasicMatrixCorrelation::CBasicMatrixCorrelation(){
}

/*
Generate a correlation matrix of the input data.

@in
- ny = number of rows
- nx = number of columns
- data = input matrix
Note: Element at row y and column x is at data[x + y*nx]

@out
- result = output correlation matrix
Note: Correlation between rows i and row j is in result[i + j*ny]
Note: Only locations where 0 <= j <= i < ny are present (avoid redundancy)
*/
void CBasicMatrixCorrelation::correlate(
    int ny,
    int nx,
    float *data,
    float *result
) {
    std::vector<double> normalised_m(nx*ny);

    // Sum-norm to 0 and then to 1
    for (auto y = 0; y < ny; y++) {
        const auto *row_start = &(data[y*nx]);

        auto row_sum = 0.;
        for (auto x = 0; x < nx; x++)
            row_sum += row_start[x];
        const auto row_mean = row_sum/nx;

        auto row_sq_sum = 0.;
        for (auto x = 0; x < nx; x++)
            row_sq_sum += (row_start[x]-row_mean)*(row_start[x]-row_mean);
        const auto row_factor = std::sqrt(row_sq_sum);

        for (auto x = 0; x < nx; x++)
            normalised_m[x + y*nx] = (row_start[x] - row_mean) / row_factor;
    }
    
    // Multiply normalised matrix by its transpose
    for (int i = 0; i < ny; ++i) {
        for (int j = 0; j <= i; ++j) {
            auto coeff = 0.;
            for (int k = 0; k < nx; ++k)
                coeff += normalised_m[k + i*nx] * normalised_m[k + j*nx];
            result[i + j*ny] = coeff;
        }
    }
}