#include <vector>
#include <cmath>
#include "CMaxOptimisedDPrecisionMatrixCorrelation.hpp"

#define V_SIZE 8
#define BLOCK_DIM 60

#if V_SIZE == 8
typedef double double8_t __attribute__ ((vector_size (8 * sizeof(double))));
inline double sum_double8_t(double8_t v) {
    return (v[0]+v[1]+v[2]+v[3]+v[4]+v[5]+v[6]+v[7]);
}
#else
typedef double double4_t __attribute__ ((vector_size (4 * sizeof(double))));
inline double sum_double4_t(double4_t v) {
    return (v[0]+v[1]+v[2]+v[3]);
}
#endif

CMaxOptimisedDPrecisionMatrixCorrelation::CMaxOptimisedDPrecisionMatrixCorrelation(){
}

/*
Generate a correlation matrix of the input data.
This implementation takes advantage of all optimisations implemented so far: ILP, OMP, and Vectorisation.
Vectorised types are double-precision.

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
void CMaxOptimisedDPrecisionMatrixCorrelation::correlate(
    int ny,
    int nx,
    float *data,
    float *result
){
    // Params
    const auto vectors_per_row = (nx + V_SIZE - 1) / V_SIZE;
    const auto n_row = (vectors_per_row + BLOCK_DIM - 1) / BLOCK_DIM;
    const auto n_col = (ny + BLOCK_DIM - 1) / BLOCK_DIM;
    const auto final_x = n_row * BLOCK_DIM;
    const auto final_y = n_col * BLOCK_DIM;
    const auto nonpad_vectors_per_row = (vectors_per_row - (nx % V_SIZE != 0 ? 1 : 0));
    const auto pad_vectors_per_row = nx % V_SIZE;
#if V_SIZE == 8
    constexpr double8_t zeros = {0.,0.,0.,0.,0.,0.,0.,0.};
#else
    constexpr double4_t zeros = {0.,0.,0.,0.};
#endif

    // Data working copy
#if V_SIZE == 8
    std::vector<double8_t> data_v(final_x*final_y, zeros);
#else
    std::vector<double4_t> data_v(final_x*final_y, zeros);
#endif

    // Populate working copy from input buffer
    #pragma omp parallel for collapse(2)
    for (auto y = 0; y < ny; y++) {
        for (auto x = 0; x < final_x; x++) {
		    for (auto k = 0; k < V_SIZE; k++) {
                const auto v_index = k + x*V_SIZE;
                if (v_index < nx)
                    data_v[x + final_x*y][k] = data[y*nx + v_index];
                else
                    data_v[x + final_x*y][k] = 0.;
		    }
	    }
    }
    
    // Norm to 0
    #pragma omp parallel for
    for (auto y = 0; y < ny; y++) {
#if V_SIZE == 8
        std::vector<double8_t> block_sum(BLOCK_DIM, zeros);
#else
        std::vector<double4_t> block_sum(BLOCK_DIM, zeros);
#endif
        for (auto i = 0; i < vectors_per_row; i+=BLOCK_DIM)
            for (auto j = 0; j < BLOCK_DIM; j++)
                block_sum[j] += data_v[i + j + final_x*y];

        auto row_sum_v = zeros;
        for (auto i = 0; i < BLOCK_DIM; i++)
            row_sum_v += block_sum[i];

#if V_SIZE == 8
        const auto row_mean = sum_double8_t(row_sum_v) / nx;
#else
        const auto row_mean = sum_double4_t(row_sum_v) / nx;
#endif

        for (auto i = 0; i < nonpad_vectors_per_row; i++)
            data_v[i + final_x*y] -= row_mean;
        for (auto i = 0; i < pad_vectors_per_row; i++)
            data_v[(y+1)*final_x - (final_x - vectors_per_row) - 1][i] -= row_mean;
    }

    // Norm to 1
    #pragma omp parallel for
    for (auto y = 0; y < ny; y++) {
#if V_SIZE == 8
        std::vector<double8_t> block_denoms(BLOCK_DIM, zeros);
#else
        std::vector<double4_t> block_denoms(BLOCK_DIM, zeros);
#endif
        for (auto i = 0; i < vectors_per_row; i+=BLOCK_DIM)
            for (auto j = 0; j < BLOCK_DIM; j++)
                block_denoms[j] += data_v[i + j + y*final_x] * data_v[i + j + y*final_x];

        auto row_denom_v = zeros;
        for (auto i = 0; i < BLOCK_DIM; i++)
            row_denom_v += block_denoms[i];
#if V_SIZE == 8
        const auto row_denom = std::sqrt(sum_double8_t(row_denom_v));
#else
        const auto row_denom = std::sqrt(sum_double4_t(row_denom_v));
#endif

        for (auto i = 0; i < nonpad_vectors_per_row; i++)
            data_v[i + final_x*y] /= row_denom;
        for (auto i = 0; i < pad_vectors_per_row; i++)
            data_v[(y+1)*final_x - (final_x - vectors_per_row) - 1][i] /= row_denom;
    }


    // Multiply norm matrix by its transpose
    // Note: we want to store results in a buffer of doubles first so as to not lose precision
    std::vector<double> result_buffer(final_y*final_y, 0.);
    
    for (auto step = 0; step < final_x; step += BLOCK_DIM) {
        #pragma omp parallel
        {
            #pragma omp for schedule(static, 1) nowait
	        for (auto y = 0; y < final_y; y += BLOCK_DIM) {
#if V_SIZE == 8
                std::vector<double8_t> block(BLOCK_DIM*BLOCK_DIM, zeros);
#else
                std::vector<double4_t> block(BLOCK_DIM*BLOCK_DIM, zeros);
#endif

	            for (auto i = 0; i < BLOCK_DIM; i++) {
		            for (auto j = 0; j < BLOCK_DIM; j++)
		                block[j + i * BLOCK_DIM] = data_v[step + j + (y + i) * final_x];
                }

	            for (auto x = y; x < final_y; x += BLOCK_DIM) {
#if V_SIZE == 8
                    std::vector<double8_t> mid_block(BLOCK_DIM*BLOCK_DIM, zeros);
#else
                    std::vector<double4_t> mid_block(BLOCK_DIM*BLOCK_DIM, zeros);
#endif
                    for (auto mid_block_row = 0; mid_block_row < BLOCK_DIM; mid_block_row++) {
                        for (auto mid_block_col = 0; mid_block_col < BLOCK_DIM; mid_block_col++) {
                            for (auto i = 0; i < BLOCK_DIM; i++)
                                mid_block[mid_block_col + mid_block_row*BLOCK_DIM] += block[i + mid_block_row*BLOCK_DIM] * data_v[i + step + (mid_block_col+x)*final_x]; // TODO: indexing mess
                        }
                    }

                    // Copy results over
		            for (auto i = 0; i < BLOCK_DIM; i++) {
		                for (auto j = 0; j < BLOCK_DIM; j++) {
			                auto acc = 0.;
			                for (auto k = 0; k < V_SIZE; k++)
			                    acc += mid_block[j + i * BLOCK_DIM][k];

			                result_buffer[j + x + (y + i) * final_y] += acc;
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