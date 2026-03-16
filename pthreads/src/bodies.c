#include "../include/bodies.h"

Body* alloc_rand_nbodies(int nbodies)
{
	Body* b_arr = malloc(nbodies*sizeof(Body));
	if(!b_arr)
	{
		return NULL;
	}

	for(int i = 0; i < nbodies; ++i)
	{
		b_arr[i] = create_rand_body();
	}

	return b_arr;

}

Body create_rand_body()
{
	return (Body)
	{
		.pos = (Vector2){rand_float(0,WIDTH),rand_float(0,HEIGHT)},
		.vel = (Vector2){rand_float(-20.0,20.0),rand_float(-20.0,20.0)},
		.acc = (Vector2){0,0},
		.r = 10.,
		.m = 10.,
		.color = rand_color(),
	};
}



Color rand_color()
{
	Color colors[] = {RAYWHITE,RED,BLUE,PURPLE,PINK,YELLOW,ORANGE,GREEN};
	return colors[rand()%8];
}

