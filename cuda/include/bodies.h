#ifndef BODIES_H
#define BODIES_H
#include <stdlib.h>
#include "raylib.h"
#include "constants.h"
#include "maths.h"
#include "time.h"
//position,velocity,acceleration,radius,mass,color
typedef struct {
	Vector2 pos;
	Vector2 vel;
	Vector2 acc;
	float r;
	float m;
	Color color;
}Body;
Body* alloc_rand_nbodies(int nbodies);
Body create_rand_body();
Color rand_color();
#endif //BODIES
