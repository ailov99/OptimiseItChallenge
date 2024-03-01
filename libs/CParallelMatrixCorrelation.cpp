#include <cmath>
#include <vector>
#include "CParallelMatrixCorrelation.hpp"

CParallelMatrixCorrelation::CParallelMatrixCorrelation(){
}

/*
Generate a correlation matrix of the input data (using OpenMP)

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
void CParallelMatrixCorrelation::correlate(
    int ny,
    int nx,
    float *data,
    float *result
) {
    std::vector<double> mean_lut(ny);

    // First find means 
    #pragma omp parallel for num_threads(20)
    for (auto i = 0; i < ny; i++) {
        // Ptr to the row
        auto *row_x = &(data[i*nx]);
        // Get mean
        auto sum_row = 0.;
        for (auto j = 0; j < nx; j++) {
            sum_row += row_x[j];
        }
        // Store mean
        mean_lut[i] = sum_row/nx;
    }

    #pragma omp parallel for num_threads(20)
    for (auto i = 0; i < ny; i++) {
        #pragma omp parallel for num_threads(20)
        for (auto j = 0; j <= i; j++) {
            // Get pointers to rows we are comparing
            auto *row_x = &(data[i*nx]);
            auto *row_y = &(data[j*nx]);

            const auto mean_row_x = mean_lut[i];
            const auto mean_row_y = mean_lut[j];

            // Get denominator and nominator
            auto nomin_sum = 0.;
            auto denom_sum_left = 0.;
            auto denom_sum_right = 0.;
            for (auto i = 0; i < nx; i++) {
                denom_sum_left += (row_x[i]-mean_row_x)*(row_x[i]-mean_row_x);
                denom_sum_right += (row_y[i]-mean_row_y)*(row_y[i]-mean_row_y);
                nomin_sum += (row_x[i]-mean_row_x)*(row_y[i]-mean_row_y);
            }

            // Compute coefficient and store
            const auto coeff = nomin_sum/(std::sqrt(denom_sum_left)*std::sqrt(denom_sum_right));
            result[i+j*ny] = static_cast<float>(coeff);
        }
    }
}