#ifndef MATHS_H
#define MATHS_H
#include "raylib.h"
#include <cuda_runtime_api.h>



#ifdef __cplusplus
extern "C" {
#endif


//if I want to use this math library outside of raylib, need to delete the include and uncomment the struct below

/***************
typedef struct {
	float x,y;
}Vector2;
***************/

//returns a random float [tmin,tmax]
float rand_float(float tmin, float tmax);
//returns a random float [tmin,tmax], if 0 was 'pulled', returns 0.0001
float rand_float_nonzero(float tmin, float tmax);
__device__ float vec2_distance_squared(Vector2 a, Vector2 b);
__device__ void vec2_add_ip(Vector2* res, Vector2 v);
__device__ void vec2_sub_ip(Vector2* res, Vector2 v);
__device__ Vector2 vec2_sub(Vector2 a, Vector2 b);
__device__ void vec2_negate_ip(Vector2* v);
__device__ void vec2_zero(Vector2* v);
__device__ float vec2_dot(Vector2 a, Vector2 b);
__device__ void vec2_scalar_mult_ip(Vector2* v, float scalar);
__device__ Vector2 vec2_scalar_mult(Vector2 v, float scalar);


#ifdef __cplusplus
}
#endif


#endif //MATHS_H

