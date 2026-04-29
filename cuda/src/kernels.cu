#include "../include/kernels.cuh"






void update_bodies(Bodies b_arr){
	reset_accelerations(b_arr); //not sure if I should para this
	accumulate_forces<<<x,y>>>(b_arr);
	move_bodies_handle_wall_collisions<<<x,y>>>(b_arr);
	handle_body_body_collisions<<<x,y>>>(b_arr);
}


__global__ void accumulate_forces(Body* b_arr, int nbodies)
{
  long i = (long)blockIdx.x * blockDim.x + threadIdx.x;
  if(i >= nbodies)
  {
    return;
  }

  
  
}
