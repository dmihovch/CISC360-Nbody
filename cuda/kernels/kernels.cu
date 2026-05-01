#include "kernels.cuh"






void update_bodies(Bodies b_arr, DoubleBuffers tmp_new_state){

  long blocks = ((b_arr.nbodies + THREADS_PER_BLOCK) - 1) / THREADS_PER_BLOCK;
  
	reset_accelerations<<<blocks,THREADS_PER_BLOCK>>>(b_arr);
	accumulate_forces<<<blocks,THREADS_PER_BLOCK>>>(b_arr);
	move_bodies_handle_wall_collisions<<<blocks,THREADS_PER_BLOCK>>>(b_arr);
	handle_body_body_collisions<<<blocks,THREADS_PER_BLOCK>>>(b_arr, tmp_new_state);
	cudaDeviceSynchronize();
	Vector2* swap_pos = tmp_new_state.pos;
	Vector2* swap_vel = tmp_new_state.vel;
	tmp_new_state.pos = b_arr.pos;
	tmp_new_state.vel = b_arr.vel;
	b_arr.pos = swap_pos;
	b_arr.vel = swap_vel;
}



__global__ void reset_accelerations(Bodies b_arr)
{
    long i = (long)blockIdx.x * blockDim.x + threadIdx.x;
  if(i >= b_arr.nbodies)
  {
    return;
  }
  vec2_zero(&b_arr.acc[i]);
}

__global__ void accumulate_forces(Bodies b_arr)
{
  long i = (long)blockIdx.x * blockDim.x + threadIdx.x;
  if(i >= b_arr.nbodies)
  {
    return;
  }

  for(int j = 0; j<b_arr.nbodies; j++)
  {
    if(i == j)
    {
        continue;    
    } 
    Vector2 delta = vec2_sub(b_arr.pos[j],b_arr.pos[i]);
    float distsq = vec2_dot(delta,delta);
    float dist = sqrtf(distsq);
    Vector2 rhat = vec2_scalar_mult(delta,1/dist);
    float grav_mass_dist = (GRAVITY * b_arr.m[i] * b_arr.m[j]) / distsq;
    Vector2 force = vec2_scalar_mult(rhat,grav_mass_dist);
    Vector2 mult_force_mass_b_i = vec2_scalar_mult(force,1/b_arr.m[i]);
    vec2_add_ip(&b_arr.acc[i],mult_force_mass_b_i);
  }
}

__global__ void move_bodies_handle_wall_collisions(Bodies b_arr)
{
  
  long i = (long)blockIdx.x * blockDim.x + threadIdx.x;
  if(i >= b_arr.nbodies)
  {
    return;
  }

  
		vec2_add_ip(&b_arr.vel[i],vec2_scalar_mult(b_arr.acc[i], DT));
		vec2_add_ip(&b_arr.pos[i],vec2_scalar_mult(b_arr.vel[i], DT));

		if(b_arr.pos[i].x + b_arr.r[i] > WIDTH)
		{
			b_arr.pos[i].x = WIDTH - b_arr.r[i];
			b_arr.vel[i].x = -b_arr.vel[i].x * ELASTICITY;
		}
		else if(b_arr.pos[i].x - b_arr.r[i] < 0)
		{
			b_arr.pos[i].x = b_arr.r[i];
			b_arr.vel[i].x = -b_arr.vel[i].x * ELASTICITY;
		}
		if(b_arr.pos[i].y + b_arr.r[i] > HEIGHT)
		{
			b_arr.pos[i].y = HEIGHT - b_arr.r[i];
			b_arr.vel[i].y = -b_arr.vel[i].y * ELASTICITY;
		}
		else if(b_arr.pos[i].y - b_arr.r[i] < 0)
		{
			b_arr.pos[i].y = b_arr.r[i];
			b_arr.vel[i].y = -b_arr.vel[i].y * ELASTICITY;
		}



  

}

__global__ void handle_body_body_collisions(Bodies b_arr, DoubleBuffers tmp_new_state)
{
  
  long i = (long)blockIdx.x * blockDim.x + threadIdx.x;
  if(i >= b_arr.nbodies)
  {
    return;
  }

  for(int j = 0; j < b_arr.nbodies; j++)
  {
    if(i == j)
    {
      continue;
    }

    float scalar_dist;
    Vector2 normal = check_collisions_circles(&scalar_dist, b_arr.pos[i], b_arr.r[i], b_arr.pos[j], b_arr.r[j]);
    if(collision_occured(normal))
    {
      //todo fix function sig
      float impulse = calculate_impulse(b_arr.vel[i],  b_arr.vel[j],b_arr.m[i], b_arr.m[j], normal);
      Vector2 impulse_vector = vec2_scalar_mult(normal,impulse);

      vec2_add_ip(&tmp_new_state.vel[i], vec2_scalar_mult(impulse_vector, 1/b_arr.m[i]));
      //need to adjust this function for cuda
      handle_penetration(&tmp_new_state.pos[i], b_arr.r[i], b_arr.r[j], b_arr.m[i], b_arr.m[j], normal, scalar_dist);

    }
  }
}
