#ifndef PHYSICS_H
#define PHYSICS_H
#include "bodies.h"
#include "constants.h"
#include "maths.h"
#include <math.h>

void update_bodies(Bodies* b);
void reset_accelerations(Bodies* b);
void accumulate_forces(Bodies* b);
void move_bodies_handle_wall_collisions(Bodies* b);
void handle_body_body_collisions(Bodies* b);
void apply_buffers(Bodies* b);
Vector2 check_collisions_circles(float* scalar_dist, Vector2 apos, float ar, Vector2 bpos, float br);
float calculate_impulse(Vector2 a_vel, Vector2 b_vel, float a_m, float b_m, Vector2 normal);
bool collision_occured(Vector2 normal);

#endif
