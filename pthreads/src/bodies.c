#include "../include/bodies.h"

void alloc_rand_nbodies(Bodies* b, Color** colors, int nbodies)
{
    b->nbodies = nbodies;
    b->pos = malloc(nbodies * sizeof(Vector2));
    b->vel = malloc(nbodies * sizeof(Vector2));
    b->acc = malloc(nbodies * sizeof(Vector2));
    b->pos_next = malloc(nbodies * sizeof(Vector2));
    b->vel_next = malloc(nbodies * sizeof(Vector2));
    b->r = malloc(nbodies * sizeof(float));
    b->m = malloc(nbodies * sizeof(float));
    *colors = malloc(nbodies * sizeof(Color));

    for(int i = 0; i < nbodies; ++i)
    {
        b->pos[i] = (Vector2){rand_float(0,WIDTH),rand_float(0,HEIGHT)};
        b->vel[i] = (Vector2){rand_float(-20.0,20.0),rand_float(-20.0,20.0)};
        b->acc[i] = (Vector2){0,0};
        b->pos_next[i] = b->pos[i];
        b->vel_next[i] = b->vel[i];
        b->r[i] = 1.0f;
        b->m[i] = 10.0f;
        (*colors)[i] = rand_color();
    }
}

void free_bodies(Bodies* b, Color* colors)
{
    free(b->pos);
    free(b->vel);
    free(b->acc);
    free(b->pos_next);
    free(b->vel_next);
    free(b->r);
    free(b->m);
    free(colors);
}

Color rand_color()
{
    Color colors[] = {RAYWHITE,RED,BLUE,PURPLE,PINK,YELLOW,ORANGE,GREEN};
    return colors[rand()%8];
}

