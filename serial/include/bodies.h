#ifndef BODIES_H
#define BODIES_H
#include <stdlib.h>
#include "raylib.h"
#include "constants.h"
#include "maths.h"
#include "time.h"

typedef struct {
    size_t nbodies;
    Vector2* pos;
    Vector2* vel;
    Vector2* acc;
    Vector2* pos_next;
    Vector2* vel_next;
    float* r;
    float* m;
} Bodies;

void alloc_rand_nbodies(Bodies* b, Color** colors, int nbodies);
void free_bodies(Bodies* b, Color* colors);
Color rand_color();

#endif
