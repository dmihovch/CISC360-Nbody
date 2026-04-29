#ifndef BODIES_H
#define BODIES_H
#include <stdlib.h>
#include "raylib.h"
#include <cuda_runtime.h>
#include "constants.h"
#include "maths.h"
#include "time.h"
//position,velocity,acceleration,radius,mass,color
// typedef struct {
// 	Vector2 pos;
// 	Vector2 vel;
// 	Vector2 acc;
// 	float r;
// 	float m;
// 	Color color;
// }Body;

//seperate array for colors
typedef struct
{
	size_t nbodies;
	Vector2* pos;
	Vector2* vel;
	Vector2* acc;
	float* r;
	float* m;
} Bodies;


void free_h_bodies(Bodies h_bodies);
void free_d_bodies(Bodies d_bodies);
int alloc_rand_nbodies_host(Bodies* h_bodies, int nbodies);
int alloc_rand_nbodies_device(Bodies* h_bodies, int nbodies);
Color rand_color();
#endif //BODIES
