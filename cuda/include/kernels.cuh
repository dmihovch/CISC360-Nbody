#include <cuda.h>

__global__ void reset_accelerations(Body* p, int nbodies);
__global__ void accumulate_forces(Body* p, int nbodies);
__global__ void move_bodies_handle_wall_collisions(Body* p, int nbodies);
__global__ void handle_body_body_collisions(Body* p, int nbodies);
