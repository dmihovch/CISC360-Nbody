#ifndef PHYSICS_H
#define PHYSICS_H

#include "bodies.h"
#include "constants.h"
#include "maths.h"
#include <math.h>
#include <pthread.h>

void init_physics_threads(Bodies* b, int n_threads);
void cleanup_physics_threads(void);
void update_bodies(Bodies* b);
void reset_accelerations(Bodies* b, size_t start, size_t end);
void accumulate_forces(Bodies* b, size_t start, size_t end);
void move_bodies_handle_wall_collisions(Bodies* b, size_t start, size_t end);
void handle_body_body_collisions(Bodies* b, size_t start, size_t end);
void apply_buffers(Bodies* b, size_t start, size_t end);
Vector2 check_collisions_circles(float* scalar_dist, Vector2 apos, float ar, Vector2 bpos, float br);
float calculate_impulse(Vector2 a_vel, Vector2 b_vel, float a_m, float b_m, Vector2 normal);
bool collision_occured(Vector2 normal);

#endif
