#include "../include/bodies.h"
//ya it was too much typing before lol
int alloc_rand_nbodies_device(Bodies* b, Vector2** old_pos, Vector2** old_vel, int n)
{
	int ops = 7;
	cudaError_t errs[ops];
	errs[0] = cudaMalloc(&b->pos,sizeof(Vector2)* n);
	errs[1] = cudaMalloc(&b->vel,sizeof(Vector2)* n);
	errs[2] = cudaMalloc(&b->acc,sizeof(Vector2)* n);
	errs[3] = cudaMalloc(&b->m,sizeof(float)*n);
	errs[4] = cudaMalloc(&b->r,sizeof(float)*n);
	errs[5] = cudaMalloc(old_pos, sizeof(Vector2)* n);
	errs[6] = cudaMalloc(old_vel, sizeof(Vector2)* n);

	for(int i = 0; i<ops; i++)
	{
		if(errs[i] != cudaSuccess)
		{
			return 1;
		}
	}


	for(int i = 0; i<n; i++)
	{
		b->pos[i] = (Vector2){rand_float(0,WIDTH),rand_float(0,HEIGHT)};
		b->vel[i] = (Vector2){rand_float(-20.0,20.0),rand_float(-20.0,20.0)};
		b->acc[i] = (Vector2){0,0};
		b->m[i] = 10.;
		b->r[i] = 10.;
	}
	memcpy(*old_pos, b->pos, n);
	memcpy(*old_vel, b->vel, n);
	b->nbodies = n;

	return 0;
	
}

int alloc_rand_nbodies_host(Bodies* h_bodies, int nbodies)
{
	h_bodies->pos = (Vector2*)malloc(sizeof(Vector2)* nbodies);
	h_bodies->vel = (Vector2*)malloc(sizeof(Vector2)* nbodies);
	h_bodies->acc = (Vector2*)malloc(sizeof(Vector2)* nbodies);
	h_bodies->m = (float*)(sizeof(float)*nbodies);
	h_bodies->r = (float*)malloc(sizeof(float)*nbodies);

	if(h_bodies->pos == NULL
	   || h_bodies->vel == NULL
		 || h_bodies->acc == NULL
		 || h_bodies->m == NULL
		 || h_bodies->r == NULL
		)
			{
				return 1;
			}
	h_bodies->nbodies = nbodies;


	return 0;
}

void safe_free(void* ptr)
{
	if(ptr != NULL)
	{
		free(ptr);
	}
}

void free_h_bodies(Bodies h_bodies)
{
	safe_free(h_bodies.pos);
	safe_free(h_bodies.vel);
	safe_free(h_bodies.acc);
	safe_free(h_bodies.m);
	safe_free(h_bodies.r);
}


void safe_cudaFree(void* dptr)
{
	if(dptr != NULL)
	{
		cudaFree(dptr);
	}
}

void free_d_bodies(Bodies d_bodies, Vector2* old_pos, Vector2* old_vel)
{
	safe_cudaFree(d_bodies.pos);
	safe_cudaFree(d_bodies.vel);
	safe_cudaFree(d_bodies.acc);
	safe_cudaFree(d_bodies.m);
	safe_cudaFree(d_bodies.r);
	safe_cudaFree(old_pos);
	safe_cudaFree(old_vel);
}

Color rand_color()
{
	Color colors[] = {RAYWHITE,RED,BLUE,PURPLE,PINK,YELLOW,ORANGE,GREEN};
	return colors[rand()%8];
}

void assign_rand_colors(Color* colors, int n)
{
	for(int i = 0; i<n; i++)
	{
		colors[i] = rand_color();
	}
}

