#include "../include/physics.h"
#include <stdlib.h>

typedef struct {
    int id;
    size_t start;
    size_t end;
    Bodies* b;
} ThreadArgs;

static pthread_barrier_t barrier_start;
static pthread_barrier_t barrier_sync;
static pthread_barrier_t barrier_end;
static bool threads_running = true;
static int num_threads = 1;
static pthread_t* threads;
static ThreadArgs* thread_args;

void* physics_worker(void* arg)
{
    ThreadArgs* args = (ThreadArgs*)arg;
    Bodies* b = args->b;
    size_t start = args->start;
    size_t end = args->end;

    while(1)
    {
        pthread_barrier_wait(&barrier_start);
        if(!threads_running) break;

        reset_accelerations(b, start, end);
        pthread_barrier_wait(&barrier_sync);

        accumulate_forces(b, start, end);
        pthread_barrier_wait(&barrier_sync);

        move_bodies_handle_wall_collisions(b, start, end);
        pthread_barrier_wait(&barrier_sync);

        handle_body_body_collisions(b, start, end);
        pthread_barrier_wait(&barrier_sync);

        apply_buffers(b, start, end);
        pthread_barrier_wait(&barrier_end);
    }
    return NULL;
}

void init_physics_threads(Bodies* b, int n_threads)
{
    num_threads = n_threads;
    pthread_barrier_init(&barrier_start, NULL, num_threads + 1);
    pthread_barrier_init(&barrier_sync, NULL, num_threads);
    pthread_barrier_init(&barrier_end, NULL, num_threads + 1);

    threads = malloc(sizeof(pthread_t) * num_threads);
    thread_args = malloc(sizeof(ThreadArgs) * num_threads);

    for(int i = 0; i < num_threads; ++i)
    {
        thread_args[i].id = i;
        thread_args[i].b = b;
        thread_args[i].start = (b->nbodies / num_threads) * i;
        thread_args[i].end = (i == num_threads - 1) ? b->nbodies : thread_args[i].start + (b->nbodies / num_threads);
        pthread_create(&threads[i], NULL, physics_worker, &thread_args[i]);
    }
}

void cleanup_physics_threads(void)
{
    threads_running = false;
    pthread_barrier_wait(&barrier_start);
    for(int i = 0; i < num_threads; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    pthread_barrier_destroy(&barrier_start);
    pthread_barrier_destroy(&barrier_sync);
    pthread_barrier_destroy(&barrier_end);
    free(threads);
    free(thread_args);
}

void update_bodies(Bodies* b)
{
    (void)b;
    pthread_barrier_wait(&barrier_start);
    pthread_barrier_wait(&barrier_end);
}

void reset_accelerations(Bodies* b, size_t start, size_t end)
{
    for(size_t i = start; i < end; ++i)
    {
        vec2_zero(&b->acc[i]);
    }
}

void accumulate_forces(Bodies* b, size_t start, size_t end)
{
    for(size_t i = start; i < end; ++i)
    {
        for(size_t j = 0; j < b->nbodies; ++j)
        {
            if(i == j) continue;
            Vector2 delta = vec2_sub(b->pos[j], b->pos[i]);
            float distsq = vec2_dot(delta, delta);
            float dist = sqrtf(distsq) + SQRTF_SOFTEN;
            Vector2 rhat = vec2_scalar_mult(delta, 1/dist);
            float grav_mass_dist = (GRAVITY * b->m[i] * b->m[j]) / distsq;
            Vector2 force = vec2_scalar_mult(rhat, grav_mass_dist);
            Vector2 force_mass_pofi = vec2_scalar_mult(force, 1/b->m[i]);
            vec2_add_ip(&b->acc[i], force_mass_pofi);
        }
    }
}

void move_bodies_handle_wall_collisions(Bodies* b, size_t start, size_t end)
{
    for(size_t i = start; i < end; ++i)
    {
        vec2_add_ip(&b->vel[i], vec2_scalar_mult(b->acc[i], DT));
        vec2_add_ip(&b->pos[i], vec2_scalar_mult(b->vel[i], DT));

        if(b->pos[i].x + b->r[i] > WIDTH)
        {
            b->pos[i].x = WIDTH - b->r[i];
            b->vel[i].x = -b->vel[i].x * ELASTICITY;
        }
        else if(b->pos[i].x - b->r[i] < 0)
        {
            b->pos[i].x = b->r[i];
            b->vel[i].x = -b->vel[i].x * ELASTICITY;
        }
        
        if(b->pos[i].y + b->r[i] > HEIGHT)
        {
            b->pos[i].y = HEIGHT - b->r[i];
            b->vel[i].y = -b->vel[i].y * ELASTICITY;
        }
        else if(b->pos[i].y - b->r[i] < 0)
        {
            b->pos[i].y = b->r[i];
            b->vel[i].y = -b->vel[i].y * ELASTICITY;
        }

        b->pos_next[i] = b->pos[i];
        b->vel_next[i] = b->vel[i];
    }
}

void handle_body_body_collisions(Bodies* b, size_t start, size_t end)
{
    for(size_t i = start; i < end; ++i)
    {
        for(size_t j = 0; j < b->nbodies; ++j)
        {
            if(i == j) continue;
            
            float scalar_dist;
            Vector2 normal = check_collisions_circles(&scalar_dist, b->pos[i], b->r[i], b->pos[j], b->r[j]);
            if(collision_occured(normal))
            {
                float impulse = calculate_impulse(b->vel[i], b->vel[j], b->m[i], b->m[j], normal);
                Vector2 impulse_vector = vec2_scalar_mult(normal, impulse);
                
                vec2_add_ip(&b->vel_next[i], vec2_scalar_mult(impulse_vector, 1/b->m[i]));
                
                float penetration_distance = b->r[i] + b->r[j];
                if(scalar_dist > 0.0f)
                {
                    penetration_distance -= scalar_dist;
                }
                Vector2 scaled_normal = vec2_scalar_mult(normal, penetration_distance * PERCENT_CORRECTION);
                float mass_for_a = b->m[j] / (b->m[i] + b->m[j]);
                Vector2 a_correction = vec2_scalar_mult(scaled_normal, mass_for_a);
                
                vec2_sub_ip(&b->pos_next[i], a_correction);
            }
        }
    }
}

void apply_buffers(Bodies* b, size_t start, size_t end)
{
    for(size_t i = start; i < end; ++i)
    {
        b->pos[i] = b->pos_next[i];
        b->vel[i] = b->vel_next[i];
    }
}

float calculate_impulse(Vector2 a_vel, Vector2 b_vel, float a_m, float b_m, Vector2 normal)
{
    Vector2 relative_vel = vec2_sub(a_vel, b_vel);
    float relative_normal = vec2_dot(relative_vel, normal);
    float inverse_masses = (1/a_m) + (1/b_m);
    float neg1plusE = -(1 + ELASTICITY);
    return (neg1plusE * relative_normal) / inverse_masses;
}

bool collision_occured(Vector2 normal)
{
    return (normal.x != 0 && normal.y != 0);
}

Vector2 check_collisions_circles(float* scalar_dist, Vector2 apos, float ar, Vector2 bpos, float br)
{
    *scalar_dist = -1;
    Vector2 delta = vec2_sub(bpos, apos);
    float distsq = vec2_dot(delta, delta);
    float rsq = (ar + br) * (ar + br);
    
    if(distsq > rsq)
    {
        return (Vector2){0, 0};
    }
    if(distsq == 0.0f)
    {
        *scalar_dist = 0.0f;
        return (Vector2){1, 0};
    }

    float dist = sqrtf(distsq) + SQRTF_SOFTEN;
    *scalar_dist = dist;
    return (Vector2){delta.x/dist, delta.y/dist};
}

