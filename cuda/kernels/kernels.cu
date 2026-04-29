#include "kernels.cuh"






void update_bodies(Bodies b_arr){

  long blocks = ((b_arr.nbodies + THREADS_PER_BLOCK) - 1) / THREADS_PER_BLOCK;
  
	reset_accelerations<<<blocks,THREADS_PER_BLOCK>>>(b_arr);
	accumulate_forces<<<blocks,THREADS_PER_BLOCK>>>(b_arr);
	move_bodies_handle_wall_collisions<<<blocks,THREADS_PER_BLOCK>>>(b_arr);
	handle_body_body_collisions<<<blocks,THREADS_PER_BLOCK>>>(b_arr);
}



__global__ void reset_accelerations(Bodies b_arr)
{
    long i = (long)blockIdx.x * blockDim.x + threadIdx.x;
  if(i >= nbodies)
  {
    return;
  }




  

}

__global__ void accumulate_forces(Bodies b_arr)
{
  long i = (long)blockIdx.x * blockDim.x + threadIdx.x;
  if(i >= nbodies)
  {
    return;
  }




  
  
}

__global__ void move_bodies_handle_wall_collisions(Bodies b_arr)
{
  
  long i = (long)blockIdx.x * blockDim.x + threadIdx.x;
  if(i >= nbodies)
  {
    return;
  }




  

}

__global__ void handle_body_body_collisions(Bodies b_arr)
{
  
  long i = (long)blockIdx.x * blockDim.x + threadIdx.x;
  if(i >= nbodies)
  {
    return;
  }





  
}
