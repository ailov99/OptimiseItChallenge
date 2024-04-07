module;

import IMatrixCorrelationStrategy;
#include <vector>
#include <cmath>

export module CILPMatrixCorrelation;

export class CILPMatrixCorrelation : public IMatrixCorrelationStrategy {
public:
    CILPMatrixCorrelation();
    auto correlate(
        const int ny,
        const int nx,
        const float* const data,
        float *result
    ) -> void override;
};

CILPMatrixCorrelation::CILPMatrixCorrelation() {
}

inline auto sum4doubles(const double doubles[4]) -> double {
    return (doubles[0] + doubles[1] + doubles[2] + doubles[3]);
}

/*
Generate a correlation matrix of the input data.
This implementation takes advantage of memoisation and instruction-level paralellism.

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
auto CILPMatrixCorrelation::correlate(
    const int ny,
    const int nx,
    const float* const data,
    float *result
) -> void {
    // Look-up table for caching the math
    typedef struct lut_record {
        double row_mean{-1.0};
        double denom_sum{-1.0};
        bool seen{false};
    } lut_record;
    std::vector<lut_record> mean_lut(ny);

    for (auto i = 0; i < ny; i++) {
        for (auto j = 0; j <= i; j++) {
            // Get pointers to rows we are comparing
            auto *row_x {&(data[i*nx])};
            auto *row_y {&(data[j*nx])};

            // Get lut records for both rows
            auto& row_x_record {mean_lut[i]};
            auto& row_y_record {mean_lut[j]};

            // Init variables
            auto mean_row_x {0.};
            auto mean_row_y {0.};
            auto denom_sum_left {0.};
            auto denom_sum_right {0.};

            // Act depending on whether we have results cached for each row
            if (row_x_record.seen) {
                mean_row_x = row_x_record.row_mean;
                denom_sum_left = row_x_record.denom_sum;
            } else {
                // Nothing in cache, so will need to compute it
                // Get mean (parallelise factor of 4)
                double sums_row_x[4] = {0.,0.,0.,0.};
                for (auto i = 0; i <= nx/4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        const auto index {i*4+j};
                        if (index < nx) {
                            sums_row_x[j] += row_x[index];
                        }
                    }  
                }
                const auto sum_row_x {sum4doubles(sums_row_x)};
                mean_row_x = sum_row_x / nx;
                // Store in cache
                row_x_record.row_mean = mean_row_x;

                // Get denominator (parallelise factor of 4)
                double denom_sums_left[4] {0.,0.,0.,0.};
                for (auto i = 0; i <= nx/4; i++) {
                    for (auto j = 0; j < 4; j++) {
                        const auto index {i*4+j};
                        if (index < nx) {
                            denom_sums_left[j] += (row_x[index]-mean_row_x)*(row_x[index]-mean_row_x);
                        }
                    }
                }
                denom_sum_left = sum4doubles(denom_sums_left);
                // Store in cache
                row_x_record.denom_sum = denom_sum_left;

                // Mark cache record as seen
                row_x_record.seen = true;
            }

            if (row_y_record.seen) {
                mean_row_y = row_y_record.row_mean;
                denom_sum_right = row_y_record.denom_sum;
            } else {
                // Nothing in cache, so will need to compute it
                // Get mean (parallelise factor of 4)
                double sums_row_y[4] {0.,0.,0.,0.};
                for (auto i = 0; i <= nx; i++) {
                    for (auto j = 0; j < 4; j++) {
                        const auto index {i*4+j};
                        if (index < nx) {
                            sums_row_y[j] += row_y[index];
                        }
                    }    
                }
                const auto sum_row_y {sum4doubles(sums_row_y)};
                mean_row_y = sum_row_y / nx;
                // Store in cache
                row_y_record.row_mean = mean_row_y;

                // Get denominator (parallelise factor of 4)
                double denom_sums_right[4] {0.,0.,0.,0.};
                for (auto i = 0; i <= nx; i++) {
                    for (auto j = 0; j < 4; j++) {
                        const auto index {i*4+j};
                        if (index < nx) {
                            denom_sums_right[j] += (row_y[index]-mean_row_y)*(row_y[index]-mean_row_y);
                        }
                    }           
                }
                denom_sum_right = sum4doubles(denom_sums_right);
                // Store in cache
                row_y_record.denom_sum = denom_sum_right;

                // Mark record as seen
                row_y_record.seen = true;
            }

            // Compute nominator (parallelise factor of 4)
            double nomin_sums[4] {0.,0.,0.,0.};
            for (auto i = 0; i <= nx/4; i++) {
                for (auto j = 0; j < 4; j++) {
                    const auto index {i*4+j};
                    if (index < nx) {
                        nomin_sums[j] += (row_x[index]-mean_row_x)*(row_y[index]-mean_row_y);
                    }
                }          
            }
            const auto nomin_sum {sum4doubles(nomin_sums)};

            // Compute coefficient and store result
            const auto coeff {nomin_sum/(std::sqrt(denom_sum_left)*std::sqrt(denom_sum_right))};
            result[i+j*ny] = static_cast<float>(coeff);
        }
    }
}