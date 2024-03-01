#include <cmath>
#include <vector>

#include "CVectorisedMatrixCorrelation.hpp"

typedef double double4_t __attribute__ ((vector_size (4 * sizeof(double))));

CVectorisedMatrixCorrelation::CVectorisedMatrixCorrelation(){
}

/*
Generate a correlation matrix of the input data, taking advantage
of vectorised types.

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
void CVectorisedMatrixCorrelation::correlate(
    int ny,
    int nx,
    float *data,
    float *result
) {
    const auto padding = nx % 4;
    const auto padded_col_size = (padding == 0) ? nx : (nx-padding+4);
    const auto vectors_per_col = padded_col_size / 4;
    const auto unpadded_nx = nx - padding;

    const double4_t zeros = {0.f, 0.f, 0.f, 0.f};
    std::vector<double4_t> data_v(ny * vectors_per_col, zeros);
    std::vector<double4_t> row_diff(ny * vectors_per_col);
    std::vector<double> row_sqr(ny);

    // Scan row-by-row
    for (auto y = 0; y < ny; y++) {
        // Populate vector of vectorised data (only where double4_t data won't be padded)
        for (auto i = 0; i < unpadded_nx; i += 4) {
            auto& data_v_v = data_v[i/4 + y*vectors_per_col];
            data_v_v[0] = data[0 + i + y*nx];
            data_v_v[1] = data[1 + i + y*nx];
            data_v_v[2] = data[2 + i + y*nx];
            data_v_v[3] = data[3 + i + y*nx];
        }

        // Vectors that contain padding are a special case - only fill up to the last element on the row
        for (auto p = 0; p < padding; p++)
            data_v[vectors_per_col - 1 + y*vectors_per_col][p] = data[p + unpadded_nx + y*nx];

        // Compute mean of the row
        double4_t row_means = zeros;
        const auto first_v_in_row = y * vectors_per_col;
        for (auto i = 0; i < vectors_per_col; i++)
            row_means += data_v[i + first_v_in_row];

        // Compute the per row average
        row_means[0] += (row_means[1] + row_means[2] + row_means[3]);
        row_means[0] /= nx;
        row_means[1] = row_means[0];
        row_means[2] = row_means[0];
        row_means[3] = row_means[0];

        // Now pad with the mean so as to not mess up the math
        if (padding != 0) {
            for (auto p = padding; p < 4; p++)
                data_v[vectors_per_col - 1 + y*vectors_per_col][p] = row_means[0];
        }

        // Compute row differences and squares
        double4_t row_sqr_sum = zeros;
        auto sums_first_v_in_row = first_v_in_row;
        for (auto i = 0; i < vectors_per_col; i++) {
            row_diff[sums_first_v_in_row] = (data_v[sums_first_v_in_row] - row_means);
            row_sqr_sum += (row_diff[sums_first_v_in_row] * row_diff[sums_first_v_in_row]);
            sums_first_v_in_row++;
        }
        row_sqr[y] = sqrt(row_sqr_sum[0] + row_sqr_sum[1] + row_sqr_sum[2] + row_sqr_sum[3]);
    }

    // Scan row-by-row and store coefficient
    for (auto y1 = 0; y1 < ny; y1++) {
        for (auto y2 = 0; y2 <= y1; y2++) {
            double4_t sum_both_rows = zeros;
            auto first_v_row_y1 = y1 * vectors_per_col;
            auto first_v_row_y2 = y2 * vectors_per_col;
            
            for (auto i = 0; i < vectors_per_col; i++)
                sum_both_rows += (row_diff[first_v_row_y1++] * row_diff[first_v_row_y2++]);
            
            const auto four_way_sums = sum_both_rows[0] + sum_both_rows[1] + sum_both_rows[2] + sum_both_rows[3];
            const auto rows_sqr_prod = row_sqr[y1] * row_sqr[y2];
            const auto coeff = four_way_sums / rows_sqr_prod;

            result[y1 + y2*ny] = coeff;
        }
    }
}