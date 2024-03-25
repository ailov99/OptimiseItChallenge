#ifdef __CUDACC__

#include <vector>
#include <stdio.h>

#include <cuda_runtime.h>
#include <helper_cuda.h>
#include <helper_functions.h>

// ======================= ERROR HANDLING =======================
static void HandleError(
    cudaError_t err, 
    const char *file, 
    int line 
) {
    if (err != cudaSuccess) {
        printf( "%s in %s at line %d\n", cudaGetErrorString( err ),
                file, line );
        exit( EXIT_FAILURE );
    }
}
#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

// ====================== CUDA ===========================
#define CHUNK_SIZE 64
#define CHUNK_DIM 8

__global__ 
void kernel_multiply(
    const float *input_m, 
    float *result, 
    const int nx, 
    const int ny,
    const int chunks_per_col
) {
    if (blockIdx.x > blockIdx.y)
        return;

    float chunk[CHUNK_SIZE];
    memset(chunk, 0.f, sizeof(chunk));
    
    for (auto x = 0; x < nx; x++) {
        float row[CHUNK_DIM];
        float col[CHUNK_DIM];

        for (auto i = 0; i < CHUNK_DIM; i++) {
            const auto x_offset = blockIdx.x*CHUNK_SIZE + i*CHUNK_DIM + threadIdx.x;
            const auto y_offset = blockIdx.y*CHUNK_SIZE + i*CHUNK_DIM + threadIdx.y;

            row[i] = input_m[chunks_per_col*x + x_offset];
            col[i] = input_m[chunks_per_col*x + y_offset];
        }

        for (auto i = 0; i < CHUNK_DIM; i++)
            for (auto j = 0; j < CHUNK_DIM; j++)
                chunk[CHUNK_DIM*i + j] += row[i] * col[j];
    }

    for (auto x = 0; x < CHUNK_DIM; x++) {
        for (auto y = 0; y < CHUNK_DIM; y++) {
            const auto real_x = CHUNK_SIZE*blockIdx.x + CHUNK_DIM*x + threadIdx.x;
            const auto real_y = CHUNK_SIZE*blockIdx.y + CHUNK_DIM*y + threadIdx.y;
            const auto in_result = (real_x < ny) && (real_y < ny);
            if (in_result)
                result[real_y + real_x*ny] = chunk[y + x*CHUNK_DIM];
        }
    }
}

__global__
void kernel_normalise(
    const float *input_m, 
    float *normalised_m, 
    const int nx, 
    const int ny
) {
    const auto y = CHUNK_SIZE * blockIdx.y + threadIdx.y;

    if (y >= ny)
        return;
    
    const auto *row_start = &(input_m[y*nx]);    

    auto row_sum = 0.f;
    for (auto x = 0; x < nx; x++)
        row_sum += row_start[x];
    const auto row_mean = row_sum/nx;

    auto row_sq_sum = 0.f;
    for (auto x = 0; x < nx; x++)
        row_sq_sum += (row_start[x]-row_mean)*(row_start[x]-row_mean);
    const auto row_factor = std::sqrt(row_sq_sum);

    for (auto x = 0; x < nx; x++)
        normalised_m[x + y*nx] = (row_start[x] - row_mean) / row_factor;
}

__global__
void kernel_xpose(
    const float *input_m,
    float *xposed_m,
    const int nx,
    const int ny,
    const int chunks_per_row,
    const int chunks_per_col
) {
    for (auto x = 0; x < chunks_per_row; x += CHUNK_SIZE) {
        const auto real_x = (x + threadIdx.x);
        const auto real_y = blockIdx.y;

        const auto xposed_index = real_x*chunks_per_col + real_y;

        const auto into_padding = (real_y >= ny) || (real_x >= nx);
        if (into_padding)
            xposed_m[xposed_index] = 0.f;
        else
            xposed_m[xposed_index] = input_m[nx*real_y + real_x];
    }
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
void correlate(
    int ny, 
    int nx, 
    const float *data, 
    float *result
) {
    // Init
    const auto chunks_per_row = ((nx + CHUNK_SIZE - 1) / CHUNK_SIZE) * CHUNK_SIZE;
    const auto chunks_per_col = ((ny + CHUNK_SIZE - 1) / CHUNK_SIZE) * CHUNK_SIZE;

    float *m_dev = nullptr;
    float *m_dev_norm = nullptr;

    HANDLE_ERROR(
        cudaMalloc(
            (void**)&m_dev_norm, 
            sizeof(float) * nx * ny
        )
    );
    HANDLE_ERROR(
        cudaMalloc(
            (void**)&m_dev, 
            sizeof(float) * nx * ny
        )
    );
    HANDLE_ERROR(
        cudaMemcpy(
            m_dev,
            data,
            sizeof(float) * nx * ny,
            cudaMemcpyHostToDevice
        )
    );

    // Norm (element-by-element)
    dim3 norm_dimBlock(1, CHUNK_SIZE);
    dim3 norm_dimGrid(1, chunks_per_col/CHUNK_SIZE);
    kernel_normalise<<<norm_dimGrid, norm_dimBlock>>>(m_dev, m_dev_norm, nx, ny);
    HANDLE_ERROR(cudaDeviceSynchronize());

    // Transpose (in chunks)
    float *m_dev_xposed = nullptr;
    HANDLE_ERROR(
        cudaMalloc(
            (void**)&m_dev_xposed,
            sizeof(float) * chunks_per_row * chunks_per_col
        )
    );
    dim3 xpose_dimBlock(CHUNK_SIZE, 1);
    dim3 xpose_dimGrid(1, chunks_per_col);
    kernel_xpose<<<xpose_dimGrid, xpose_dimBlock>>>(m_dev_norm, m_dev_xposed, nx, ny, chunks_per_row, chunks_per_col);
    HANDLE_ERROR(cudaDeviceSynchronize());


    // Multiply (in chunks)
    float *result_buffer_dev = nullptr;
    HANDLE_ERROR(
        cudaMalloc(
            (void**)&result_buffer_dev, 
            sizeof(float) * ny * ny
        )
    );
    HANDLE_ERROR(
        cudaMemset(
            result_buffer_dev,
            0.f,
            sizeof(float) * ny * ny
        )
    );

    dim3 mult_dimBlock(CHUNK_DIM, CHUNK_DIM);
    dim3 mult_dimGrid((chunks_per_col / CHUNK_SIZE),(chunks_per_col / CHUNK_SIZE));
    kernel_multiply<<<mult_dimGrid, mult_dimBlock>>>(m_dev_xposed, result_buffer_dev, nx, ny, chunks_per_col);
    HANDLE_ERROR(cudaDeviceSynchronize());

    HANDLE_ERROR(
        cudaMemcpy(
            result,
            result_buffer_dev,
            sizeof(float) * ny * ny,
            cudaMemcpyDeviceToHost
        )
    );

    HANDLE_ERROR(cudaFree(m_dev));
    HANDLE_ERROR(cudaFree(m_dev_norm));
    HANDLE_ERROR(cudaFree(m_dev_xposed));
    HANDLE_ERROR(cudaFree(result_buffer_dev));
}

#endif