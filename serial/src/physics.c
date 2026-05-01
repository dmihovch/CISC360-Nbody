#include "../include/physics.h"

void update_bodies(Bodies* b)
{
    reset_accelerations(b);
    accumulate_forces(b);
    move_bodies_handle_wall_collisions(b);
    handle_body_body_collisions(b);
}

void reset_accelerations(Bodies* b)
{
    for(size_t i = 0; i < b->nbodies; ++i)
    {
        vec2_zero(&b->acc[i]);
    }
}

void accumulate_forces(Bodies* b)
{
    for(size_t i = 0; i < b->nbodies; ++i)
    {
        for(size_t j = i + 1; j < b->nbodies; ++j)
        {
            Vector2 delta = vec2_sub(b->pos[j], b->pos[i]);
            float distsq = vec2_dot(delta, delta);
            float dist = sqrtf(distsq) + SQRTF_SOFTEN;
            Vector2 rhat = vec2_scalar_mult(delta, 1/dist);
            float grav_mass_dist = (GRAVITY * b->m[i] * b->m[j]) / distsq;
            Vector2 force = vec2_scalar_mult(rhat, grav_mass_dist);
            Vector2 force_mass_pofi = vec2_scalar_mult(force, 1/b->m[i]);
            Vector2 force_mass_pofj = vec2_scalar_mult(force, 1/b->m[j]);
            vec2_add_ip(&b->acc[i], force_mass_pofi);
            vec2_sub_ip(&b->acc[j], force_mass_pofj);
        }
    }
}

void move_bodies_handle_wall_collisions(Bodies* b)
{
    for(size_t i = 0; i < b->nbodies; ++i)
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
    }
}

void handle_body_body_collisions(Bodies* b)
{
    for(size_t i = 0; i < b->nbodies; ++i)
    {
        for(size_t j = i + 1; j < b->nbodies; ++j)
        {
            float scalar_dist;
            Vector2 normal = check_collisions_circles(&scalar_dist, b->pos[i], b->r[i], b->pos[j], b->r[j]);
            if(collision_occured(normal))
            {
                float impulse = calculate_impulse(b->vel[i], b->vel[j], b->m[i], b->m[j], normal);
                Vector2 impulse_vector = vec2_scalar_mult(normal, impulse);
                
                vec2_add_ip(&b->vel[i], vec2_scalar_mult(impulse_vector, 1/b->m[i]));
                vec2_add_ip(&b->vel[j], vec2_scalar_mult(impulse_vector, -(1/b->m[j])));
                
                handle_penetration(&b->pos[i], &b->pos[j], b->r[i], b->r[j], b->m[i], b->m[j], normal, scalar_dist);
            }
        }
    }
}

void handle_penetration(Vector2* a_pos, Vector2* b_pos, float a_r, float b_r, float a_m, float b_m, Vector2 normal, float scalar_distance)
{
    float penetration_distance = a_r + b_r;
    if(scalar_distance > 0.0f)
    {
        penetration_distance -= scalar_distance;
    }
    Vector2 scaled_normal = vec2_scalar_mult(normal, penetration_distance * PERCENT_CORRECTION);

    float mass_for_a = b_m / (a_m + b_m);
    float mass_for_b = a_m / (a_m + b_m);

    Vector2 a_correction = vec2_scalar_mult(scaled_normal, mass_for_a);
    Vector2 b_correction = vec2_scalar_mult(scaled_normal, mass_for_b);

    vec2_sub_ip(a_pos, a_correction);
    vec2_add_ip(b_pos, b_correction);
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
