#ifndef PHYSICS_H
#define PHYSICS_H
#include "maths.cuh"
#include "constants.h"

#ifdef __cplusplus
extern "C" {
#endif

__device__ Vector2 check_collisions_circles(float* scalar_dist,Vector2 apos, float ar, Vector2 bpos, float br);
__device__ float calculate_impulse(Vector2 a_vel, Vector2 b_vel, float a_m, float b_m, Vector2 normal);
__device__ bool collision_occured(Vector2 normal);
__device__ void handle_penetration(Vector2* i_pos, float i_rad,float j_rad, float i_mass, float j_mass,Vector2 normal, float scalar_distance);


#ifdef __cplusplus
}
#endif


#endif //PHYSICS_H

