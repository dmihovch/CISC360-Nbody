#ifndef KERNELS_CUH
#define KERNELS_CUH

#include <cuda.h>
#include <cuda_runtime.h>
#include "../include/bodies.h"
#include "../include/physics.cuh"

#define THREADS_PER_BLOCK 256
void update_bodies(Bodies b, DoubleBuffers old);
__global__ void reset_accelerations(Bodies b);
__global__ void accumulate_forces(Bodies b);
__global__ void move_bodies_handle_wall_collisions(Bodies b);
__global__ void handle_body_body_collisions(Bodies b, DoubleBuffers old);

#endif //KERNELS
