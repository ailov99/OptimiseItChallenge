module;

import IMatrixCorrelationStrategy;
#include <vector>
#include <cmath>

export module CMaxOptimisedSPrecisionMatrixCorrelation;

export class CMaxOptimisedSPrecisionMatrixCorrelation : public IMatrixCorrelationStrategy {
public:
    CMaxOptimisedSPrecisionMatrixCorrelation();
    auto correlate(
        const int ny,
        const int nx,
        const float* const data,
        float *result
    ) -> void override;   
};

namespace SPrecisionMatrixCorrelationUtils {
    typedef float float8_t __attribute__ ((vector_size (8*sizeof(float))));

    inline auto sum_float8_t(const float8_t v) -> float {
       return (v[0]+v[1]+v[2]+v[3]+v[4]+v[5]+v[6]+v[7]);
    } 
}

CMaxOptimisedSPrecisionMatrixCorrelation::CMaxOptimisedSPrecisionMatrixCorrelation(){
}

/*
Generate a correlation matrix of the input data.
This implementation takes advantage of all optimisations implemented so far: ILP, OMP, and Vectorisation.
Vectorised types are single-precision.

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
auto CMaxOptimisedSPrecisionMatrixCorrelation::correlate(
    const int ny,
    const int nx,
    const float* const data,
    float *result
) -> void {
    using namespace SPrecisionMatrixCorrelationUtils;
    constexpr auto V_SIZE {8};
    constexpr auto BLOCK_DIM {64};

    // Params
    const auto vectors_per_row {(nx + V_SIZE - 1) / V_SIZE};
    const auto n_row {(vectors_per_row + BLOCK_DIM - 1) / BLOCK_DIM};
    const auto n_col {(ny + BLOCK_DIM - 1) / BLOCK_DIM};
    const auto final_x {n_row * BLOCK_DIM};
    const auto final_y {n_col * BLOCK_DIM};
    const auto nonpad_vectors_per_row {(vectors_per_row - (nx % V_SIZE != 0 ? 1 : 0))};
    const auto pad_vectors_per_row {nx % V_SIZE};
    constexpr float8_t zeros {0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f};

    // Data working copy
    std::vector<float8_t> data_v(final_x*final_y, zeros);

    // Populate working copy from input buffer
    #pragma omp parallel for collapse(2)
    for (auto y = 0; y < ny; y++) {
        for (auto x = 0; x < final_x; x++) {
		    for (auto k = 0; k < V_SIZE; k++) {
                const auto v_index {k + x*V_SIZE};
                if (v_index < nx) {
                    data_v[x + final_x*y][k] = data[y*nx + v_index];
                } else {
                    data_v[x + final_x*y][k] = 0.f;
                }
		    }
	    }
    }
    
    // Norm to 0
    #pragma omp parallel for schedule(static,1)
    for (auto y = 0; y < ny; y++) {
        std::vector<float8_t> block_sum(BLOCK_DIM, zeros);

        for (auto i = 0; i < vectors_per_row; i+=BLOCK_DIM)
            for (auto j = 0; j < BLOCK_DIM; j++)
                block_sum[j] += data_v[i + j + final_x*y];

        auto row_sum_v {zeros};
        for (auto i = 0; i < BLOCK_DIM; i++)
            row_sum_v += block_sum[i];

        const auto row_mean {sum_float8_t(row_sum_v) / nx};

        for (auto i = 0; i < nonpad_vectors_per_row; i++)
            data_v[i + final_x*y] -= row_mean;
        for (auto i = 0; i < pad_vectors_per_row; i++)
            data_v[(y+1)*final_x - (final_x - vectors_per_row) - 1][i] -= row_mean;
    }

    // Norm to 1
    #pragma omp parallel for schedule(static,1)
    for (auto y = 0; y < ny; y++) {
        std::vector<float8_t> block_denoms(BLOCK_DIM, zeros);

        for (auto i = 0; i < vectors_per_row; i+=BLOCK_DIM)
            for (auto j = 0; j < BLOCK_DIM; j++)
                block_denoms[j] += (data_v[i + j + y*final_x] * data_v[i + j + y*final_x]);

        auto row_denom_v {zeros};
        for (auto i = 0; i < BLOCK_DIM; i++)
            row_denom_v += block_denoms[i];

        const auto row_denom {std::sqrt(sum_float8_t(row_denom_v))};

        for (auto i = 0; i < nonpad_vectors_per_row; i++)
            data_v[i + final_x*y] /= row_denom;
        for (auto i = 0; i < pad_vectors_per_row; i++)
            data_v[(y+1)*final_x - (final_x - vectors_per_row) - 1][i] /= row_denom;
    }


    // Multiply norm matrix by its transpose
    std::vector<float> result_buffer(final_y*final_y, 0.f);
    
    for (auto step = 0; step < final_x; step += BLOCK_DIM) {
        #pragma omp parallel
        {
            #pragma omp for schedule(static,1) nowait
	        for (auto y = 0; y < final_y; y += BLOCK_DIM) {

                std::vector<float8_t> block(BLOCK_DIM*BLOCK_DIM, zeros);

	            for (auto i = 0; i < BLOCK_DIM; i++) {
		            for (auto j = 0; j < BLOCK_DIM; j++)
		                block[j + i * BLOCK_DIM] = data_v[step + j + (y + i) * final_x];
                }

	            for (auto x = y; x < final_y; x += BLOCK_DIM) {
                    std::vector<float8_t> mid_block(BLOCK_DIM*BLOCK_DIM, zeros);

                    for (auto mid_block_row = 0; mid_block_row < BLOCK_DIM; mid_block_row++) {
                        for (auto mid_block_col = 0; mid_block_col < BLOCK_DIM; mid_block_col++) {
                            for (auto i = 0; i < BLOCK_DIM; i++)
                                mid_block[mid_block_col + mid_block_row*BLOCK_DIM] += block[i + mid_block_row*BLOCK_DIM] * data_v[i + step + (mid_block_col+x)*final_x];
                        }
                    }

                    // Copy results over
		            for (auto i = 0; i < BLOCK_DIM; i++) {
		                for (auto j = 0; j < BLOCK_DIM; j++) {
			                for (auto k = 0; k < V_SIZE; k++)
			                    result_buffer[j + x + (y + i) * final_y] += mid_block[j + i * BLOCK_DIM][k];
		                }
		            }
	            }
	        }
        }
    }
    
    // Copy buffered results over to the float* result out
    #pragma omp parallel
    { 
        #pragma omp for collapse(2) nowait
        for (auto y = 0; y < ny; y++)
	        for (auto x = y; x < ny; x++)
	            result[x + y * ny] = result_buffer[x + y * final_y];
    }
}