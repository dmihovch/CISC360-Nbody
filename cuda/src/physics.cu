#include "../include/physics.cuh"

__device__ void handle_penetration(Vector2* i_pos, float i_rad, float j_rad, float i_mass, float j_mass ,Vector2 normal, float scalar_distance)
{
	float penetration_distance = i_rad + j_rad;
	if(scalar_distance > 0.)
	{
		penetration_distance -= scalar_distance;
	}
	Vector2 scaled_normal = vec2_scalar_mult(normal, penetration_distance*PERCENT_CORRECTION);

	float mass_for_i = j_mass/(i_mass + j_mass);

	Vector2 i_correction = vec2_scalar_mult(scaled_normal, mass_for_i);

	vec2_sub_ip(i_pos, i_correction);
}
__device__ float calculate_impulse(Vector2 a_vel, Vector2 b_vel, float a_m, float b_m,  Vector2 normal)
{
	Vector2 relative_vel = vec2_sub(a_vel, b_vel);
	float relative_normal = vec2_dot(relative_vel,normal);
	float inverse_masses = (1/a_m) + (1/b_m);
	float neg1plusE = -(1 + ELASTICITY);
	return (neg1plusE * relative_normal) / inverse_masses;
}

__device__ bool collision_occured(Vector2 normal)
{
	return (normal.x != 0 && normal.y != 0);
}

__device__ Vector2 check_collisions_circles(float* scalar_dist,Vector2 apos, float ar, Vector2 bpos, float br)
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
