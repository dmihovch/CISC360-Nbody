#ifndef PHYSICS_H
#define PHYSICS_H
#include "bodies.h"
#include "constants.h"
#include "maths.h"
#include <math.h>

__device__ Vector2 check_collisions_circles(float* scalar_dist,Vector2 apos, float ar, Vector2 bpos, float br);
__device__ float calculate_impulse(Body a, Body b, Vector2 normal);
__device__ bool collision_occured(Vector2 normal);
__device__ void handle_penetration(Body* a, Body* b,Vector2 normal, float scalar_distance);
#endif //PHYSICS_H
