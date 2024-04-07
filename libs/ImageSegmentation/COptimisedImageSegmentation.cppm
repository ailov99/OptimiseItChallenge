module;

#include <vector>
import IImageSegmentationStrategy;

export module COptimisedImageSegmentation;

namespace ImageSegmentationVectorisedUtils {
    typedef double double4_t __attribute__ ((vector_size (4 * sizeof(double))));
    inline auto sum_double4_t(double4_t v) -> double {
        return (v[0]+v[1]+v[2]+v[3]);
    }
}

export class COptimisedImageSegmentation : public IImageSegmentationStrategy {
public:
    COptimisedImageSegmentation();
    auto segment(
        const int ny,
        const int nx,
        const float* const in_data
    ) -> SegmentationDescription override;
};

COptimisedImageSegmentation::COptimisedImageSegmentation() {
}

/*
Segment an image into 2 parts:
1. A monochromatic triangle
2. A monochromatic background

Colour components, are numbered 0 to 2 (R,G,B).
x coordinates are numbered 0 to (nx-1).
y coordinates are numbered 0 to (ny-1).
A colour component is stored at `data[c + 3*x + 3*nx*y]` where `c` is 0 for R, 1 for G, 2 for B.

@in
- ny = image height
- nx = image width
- in_data = a colour image with `ny*nx` pixels, each pixel consisting of an R, G, and B component (ie size is ny*nx*3)

@return
A structure describing the location of the monochromatic triangle and colour components of both the background and rectangle
*/
auto COptimisedImageSegmentation::segment(
    const int ny,
    const int nx,
    const float* const in_data
) -> SegmentationDescription {
    using namespace ImageSegmentationVectorisedUtils;

    SegmentationDescription result{0,0,0,0,{0.f,0.f,0.f},{0.f,0.f,0.f}};
    constexpr double4_t zeros{0.,0.,0.,0.};
    std::vector<double4_t> data_v(nx*ny, zeros);
    
    // Input data to vectorised form (each pixel has its own vector containing RGB)
    #pragma omp for collapse(2)
    for (auto y = 0; y < ny; y++)
        for (auto x = 0; x < nx; x++)
            for (auto i = 0; i < 3; i++)
                data_v[x + nx*y][i] = in_data[(x + nx*y)*3 + i];

    // Now we want to build a lookup for constant time sum access (can't omp this)
    std::vector<double4_t> sum_lookup((nx+1)*(ny+1), zeros);
    for (auto y = 1; y <= ny; y++) {
        for (auto x = 1; x <= nx; x++) {
            sum_lookup[x + (nx+1)*y] = 
                data_v[x-1 + nx*(y-1)] +
                sum_lookup[x-1 + (nx+1)*y] + 
                sum_lookup[x + (nx+1)*(y-1)] - 
                sum_lookup[x-1 + (nx+1)*(y-1)];
        }
    }

    const auto p_sum{sum_lookup[nx + (nx+1)*ny]};
    struct win_dims {
        int top_left_x;
        int top_left_y;
        int bot_right_x;
        int bot_right_y;
    };
    auto overall_max{0.};

    // Now multithread again
    #pragma omp parallel
    {
        win_dims this_win_dims{0,0,0,0};
        auto this_max{0.};
        
        for (auto this_height = 1; this_height <= ny; this_height++) {
            #pragma omp for schedule(dynamic)
            for (auto this_width = 1; this_width <= nx; this_width++) {
                const auto x{this_height*this_width};
                const auto y{nx*ny - x};
                const auto x_d{1./x};
                const auto y_d{1./y};

                for (auto y0 = 0; y0 <= ny-this_height; y0++) {
                    for (auto x0 = 0; x0 <= nx-this_width; x0++) {
                        const auto y1{y0 + this_height};
                        const auto x1{x0 + this_width};

                        const auto x_v{ 
                            sum_lookup[x1 + (nx + 1) * y1] - 
                            sum_lookup[x0 + (nx + 1) * y1] - 
                            sum_lookup[x1 + (nx + 1) * y0] + 
                            sum_lookup[x0 + (nx + 1) * y0]
                        };

                        const auto y_v{p_sum - x_v};
                        const auto h_x_y{x_v * x_v * x_d + y_v * y_v * y_d};
                        const auto current_max{sum_double4_t(h_x_y)};

                        if (current_max > this_max) {
                            this_max = current_max;
                            this_win_dims = {x0, y0, x1, y1};
                        }
                    }
                }
            }
        }

        // Global result update
        #pragma omp critical
        {
            if (this_max > overall_max) {
                overall_max = this_max;
                result.x0 = this_win_dims.top_left_x;
                result.y0 = this_win_dims.top_left_y;
                result.x1 = this_win_dims.bot_right_x;
                result.y1 = this_win_dims.bot_right_y;
            }
        }
    }

    // Compute the end result dims
    auto x_v{
        sum_lookup[result.x1 + (nx + 1) * result.y1] - 
        sum_lookup[result.x0 + (nx + 1) * result.y1] - 
        sum_lookup[result.x1 + (nx + 1) * result.y0] + 
        sum_lookup[result.x0 + (nx + 1) * result.y0]
    };
    auto y_v{p_sum - x_v};

    const auto x{(result.y1 - result.y0) * (result.x1 - result.x0)};
    const auto y{ny*nx - x};

    x_v /= x;
    y_v /= y;

    for (auto i = 0; i < 3; i++) {
        result.inner[i] = x_v[i];
        result.outer[i] = y_v[i];
    }

    return result;
}