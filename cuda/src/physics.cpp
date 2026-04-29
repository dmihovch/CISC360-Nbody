#include "../include/physics.h"


void reset_accelerations(Body* b_arr, int nbodies)
{
	for(int i = 0; i<nbodies; ++i)
	{
		vec2_zero(&b_arr[i].acc);
	}
}

void accumulate_forces(Body* b_arr, int nbodies)
{
	for(int i = 0; i<nbodies; ++i)
	{
		for(int j = i + 1; j<nbodies; ++j)
		{
			//calculate the forces for both i and j
			//inverse square law
			Vector2 delta = vec2_sub(b_arr[j].pos, b_arr[i].pos);
			float distsq = vec2_dot(delta, delta);
			float dist = sqrtf(distsq);
			Vector2 rhat = vec2_scalar_mult(delta, 1/dist);
			float grav_mass_dist = (GRAVITY*b_arr[i].m*b_arr[j].m) / distsq;
			Vector2 force = vec2_scalar_mult(rhat, grav_mass_dist);
			Vector2 force_mass_pofi = vec2_scalar_mult(force, 1/b_arr[i].m);
			Vector2 force_mass_pofj = vec2_scalar_mult(force, 1/b_arr[j].m);
			vec2_add_ip(&b_arr[i].acc, force_mass_pofi);
			vec2_sub_ip(&b_arr[j].acc, force_mass_pofj);
		}
	}
}

void move_bodies_handle_wall_collisions(Body* b_arr, int nbodies)
{
	for(int i = 0; i<nbodies; ++i)
	{
		vec2_add_ip(&b_arr[i].vel,vec2_scalar_mult(b_arr[i].acc, DT));
		vec2_add_ip(&b_arr[i].pos,vec2_scalar_mult(b_arr[i].vel, DT));

		if(b_arr[i].pos.x + b_arr[i].r > WIDTH)
		{
			b_arr[i].pos.x = WIDTH - b_arr[i].r;
			b_arr[i].vel.x = -b_arr[i].vel.x * ELASTICITY;
		}
		else if(b_arr[i].pos.x - b_arr[i].r < 0)
		{
			b_arr[i].pos.x = b_arr[i].r;
			b_arr[i].vel.x = -b_arr[i].vel.x * ELASTICITY;
		}
		if(b_arr[i].pos.y + b_arr[i].r > HEIGHT)
		{
			b_arr[i].pos.y = HEIGHT - b_arr[i].r;
			b_arr[i].vel.y = -b_arr[i].vel.y * ELASTICITY;
		}
		else if(b_arr[i].pos.y - b_arr[i].r < 0)
		{
			b_arr[i].pos.y = b_arr[i].r;
			b_arr[i].vel.y = -b_arr[i].vel.y * ELASTICITY;
		}
	}

}

void handle_body_body_collisions(Body* b_arr, int nbodies)
{
	for(int i = 0; i < nbodies; ++i)
	{
		for(int j = i+1; j<nbodies; ++j)
		{
			float scalar_dist;	
			Vector2 normal = check_collisions_circles(&scalar_dist,b_arr[i].pos, b_arr[i].r, b_arr[j].pos, b_arr[j].r);
			if(collision_occured(normal)){
				float impulse = calculate_impulse(b_arr[i], b_arr[j], normal);
				Vector2 impulse_vector = vec2_scalar_mult(normal, impulse);
				//this is messy, I don't like it, but it's not the point
				vec2_add_ip(&b_arr[i].vel, vec2_scalar_mult(impulse_vector, 1/b_arr[i].m));
				vec2_add_ip(&b_arr[j].vel, vec2_scalar_mult(impulse_vector, -(1/b_arr[j].m)));
				handle_penetration(&b_arr[i], &b_arr[j], normal, scalar_dist);
			}
		}
	}
}

void handle_penetration(Body* a, Body* b,Vector2 normal, float scalar_distance)
{
	float penetration_distance = a->r + b->r;
	if(scalar_distance > 0.)
	{
		penetration_distance -= scalar_distance;
	}
	Vector2 scaled_normal = vec2_scalar_mult(normal, penetration_distance*PERCENT_CORRECTION);

	float mass_for_a = b->m/(a->m + b->m);
	float mass_for_b = a->m/(a->m + b->m);

	Vector2 a_correction = vec2_scalar_mult(scaled_normal, mass_for_a);
	Vector2 b_correction = vec2_scalar_mult(scaled_normal, mass_for_b);

	vec2_sub_ip(&a->pos, a_correction);
	vec2_add_ip(&b->pos, b_correction);
}
float calculate_impulse(Body a, Body b, Vector2 normal)
{
	Vector2 relative_vel = vec2_sub(a.vel, b.vel);
	float relative_normal = vec2_dot(relative_vel,normal);
	float inverse_masses = (1/a.m) + (1/b.m);
	float neg1plusE = -(1 + ELASTICITY);
	return (neg1plusE * relative_normal) / inverse_masses;
}

bool collision_occured(Vector2 normal)
{
	return (normal.x != 0 && normal.y != 0);
}

Vector2 check_collisions_circles(float* scalar_dist,Vector2 apos, float ar, Vector2 bpos, float br)
{

	*scalar_dist = -1;
	Vector2 delta = vec2_sub(bpos, apos);
	float distsq = vec2_dot(delta,delta);
	float rsq = (ar+br) * (ar+br);
	if(distsq > rsq)
	{
		return (Vector2){0,0};
	}
	if(distsq == 0.)
	{
		//not sure if I have to calculate the peenetration of this case
		*scalar_dist = 0.;
		return (Vector2)
		{
			1,0
		};
	} 

	float dist = sqrtf(distsq);
	*scalar_dist = dist;
	return (Vector2){delta.x/dist, delta.y/dist};
}
