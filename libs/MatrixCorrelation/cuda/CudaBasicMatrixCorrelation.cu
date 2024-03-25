#ifdef __CUDACC__

#include <vector>
#include <stdio.h>

#include <cuda_runtime.h>
#include <helper_cuda.h>
#include <helper_functions.h>

// ======================= ERROR HANDLING =======================
static void HandleError(cudaError_t err, const char *file, int line ) {
    if (err != cudaSuccess) {
        printf( "%s in %s at line %d\n", cudaGetErrorString( err ),
                file, line );
        exit( EXIT_FAILURE );
    }
}
#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

// ====================== CUDA ===========================
__global__ 
void mykernel(float *normalised_m, float *result, int nx, int ny) {
    const auto x_dim = threadIdx.x + blockIdx.x * blockDim.x;
    const auto y_dim = threadIdx.y + blockIdx.y * blockDim.y;

    if (x_dim >= ny || y_dim >= ny)
        return;

    auto coeff = 0.f;
    for (auto x = 0; x < nx; x++)
        coeff += normalised_m[x + y_dim*nx] * normalised_m[x + x_dim*nx];
    result[x_dim*ny + y_dim] = coeff;
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
void correlate(int ny, int nx, const float *data, float *result) {
    float *normalised_m_host = new float[ny*nx];
    
    // Sum-norm to 0 and then to 1
    for (auto y = 0; y < ny; y++) {
        const auto *row_start = &(data[y*nx]);

        auto row_sum = 0.f;
        for (auto x = 0; x < nx; x++)
            row_sum += row_start[x];
        const auto row_mean = row_sum/nx;

        auto row_sq_sum = 0.f;
        for (auto x = 0; x < nx; x++)
            row_sq_sum += (row_start[x]-row_mean)*(row_start[x]-row_mean);
        const auto row_factor = std::sqrt(row_sq_sum);

        for (auto x = 0; x < nx; x++)
            normalised_m_host[x + y*nx] = (row_start[x] - row_mean) / row_factor;
    }

    float *normalised_m_dev = nullptr;
    HANDLE_ERROR(
        cudaMalloc(
            (void**)&normalised_m_dev, 
            sizeof(float) * nx * ny
        )
    );
    float *result_buffer_dev = nullptr;
    HANDLE_ERROR(
        cudaMalloc(
            (void**)&result_buffer_dev, 
            sizeof(float) * ny * ny
        )
    );
    HANDLE_ERROR(
        cudaMemcpy(
            normalised_m_dev,
            normalised_m_host,
            sizeof(float) * nx * ny,
            cudaMemcpyHostToDevice
        )
    );

    dim3 dimBlock(16, 16);
    dim3 dimGrid( (ny + dimBlock.x - 1)/dimBlock.x , (ny + dimBlock.y - 1)/dimBlock.y );
    mykernel<<<dimGrid, dimBlock>>>(normalised_m_dev, result_buffer_dev, nx, ny);
    HANDLE_ERROR(cudaDeviceSynchronize());

    HANDLE_ERROR(
        cudaMemcpy(
            result,
            result_buffer_dev,
            sizeof(float) * ny * ny,
            cudaMemcpyDeviceToHost
        )
    );

    HANDLE_ERROR(cudaFree(normalised_m_dev));
    HANDLE_ERROR(cudaFree(result_buffer_dev));
    delete[] normalised_m_host;
}

#endif