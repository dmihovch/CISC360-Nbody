#include "include/drawing.h"
#include "kernels/kernels.cuh"
#include "include/constants.h"
#include "include/bodies.h"
#include <cuda_runtime_api.h>
#include <raylib.h>
#include <cuda.h>



int main(int argc, char** argv){

	srand(420);
	int nbodies = BODIES;
	if(argc >= 2)
	{
		int argv_bodies = atoi(argv[1]);
		if(argv_bodies >= 1)
		{
			nbodies = argv_bodies;
		}
		else{
			printf("Your input couldn't be parsed, defaulting to %d bodies\n",nbodies);
		}
	}
	else{
		printf("No body count passed to program\nUsing default value of %d bodies\nUsage: ./serial <body count>\n",nbodies);
	}
	int err;
	Bodies h_bodies;
	err = alloc_rand_nbodies_host(&h_bodies, nbodies);
	if(err)
	{
		printf("Failed to allocate %d bodies\n",nbodies);
		free_h_bodies(h_bodies);
		return 1;
	}

	Bodies d_bodies;
	DoubleBuffers tmp_new_bufs;
	err = alloc_rand_nbodies_device(&d_bodies, &tmp_new_bufs, nbodies);
	if(err)
	{
		printf("Failed to allocate %d bodies on GPU\n",nbodies);
		free_d_bodies(d_bodies, tmp_new_bufs);
		free_h_bodies(h_bodies);
		return 1;
	}


	Color* colors = (Color*)malloc(sizeof(Color)*nbodies);
	if(colors == NULL)
	{
		free_h_bodies(h_bodies);
		free_d_bodies(d_bodies, tmp_new_bufs);
		return 1;
	}
	assign_rand_colors(colors,nbodies);

	InitWindow(WIDTH, HEIGHT, "N-Body Simulation [ SERIAL ]");
	if(!IsWindowReady())
	{
		free_d_bodies(d_bodies, tmp_new_bufs);
		free_h_bodies(h_bodies);
		free(colors);
		return 1;
	}

	cudaMemcpy(d_bodies.pos,h_bodies.pos,sizeof(Vector2)*nbodies,cudaMemcpyHostToDevice);
	cudaMemcpy(d_bodies.vel, h_bodies.vel, sizeof(Vector2)*nbodies,cudaMemcpyHostToDevice);
	cudaMemcpy(d_bodies.acc, h_bodies.acc, sizeof(Vector2)*nbodies,cudaMemcpyHostToDevice);
	cudaMemcpy(d_bodies.r, h_bodies.r, sizeof(float)*nbodies,cudaMemcpyHostToDevice);
	cudaMemcpy(d_bodies.m, h_bodies.m, sizeof(float)*nbodies,cudaMemcpyHostToDevice);
	double hotpath_memcpy_start = 0;
	double hotpath_memcpy_end = 0;

	double frametime_start = 0;
	double frametime_end = 0;

	double render_start = 0;
	double render_end = 0;

	double update_start = 0;
	double update_end = 0;
	
	double total_frame_time = 0;
	double total_update_time = 0;
	double total_hotpath_memcpy_time = 0;
	long long total_frames = 0;

	double average_frame_time = 0;
	double average_update_time = 0;
	double average_hotpath_memcpy_time = 0;
  //SetTargetFPS(FPS);
	while(!WindowShouldClose() && GetKeyPressed() != KEY_Q && GetTime() <= 10)
	{

		frametime_start = GetTime();

		update_start = GetTime();
		update_bodies(&d_bodies, &tmp_new_bufs);

		//errors
		if(cudaDeviceSynchronize() != cudaSuccess)
		{
			goto free_and_exit;
		}
		update_end = GetTime();

		hotpath_memcpy_start = GetTime();

		//errors
		if(cudaMemcpy(h_bodies.pos, d_bodies.pos, sizeof(Vector2)*nbodies, cudaMemcpyDeviceToHost) != cudaSuccess) {
				goto free_and_exit;
			}

		hotpath_memcpy_end = GetTime();

		render_start = GetTime();
		BeginDrawing();
		ClearBackground(BLACK);
		draw_bodies(h_bodies, colors);
		render_end = GetTime();

		DrawFPS(0, 0);



		//draw_diagnostics(frametime_start, frametime_end, render_start, render_end, update_start, update_end, nbodies);

		EndDrawing();

		frametime_end = GetTime();

		total_frame_time += (frametime_end - frametime_start);
		total_update_time += (update_end - update_start);
		total_hotpath_memcpy_time += (hotpath_memcpy_end - hotpath_memcpy_start);
		total_frames++;
	}


	average_frame_time = (total_frame_time * 1000) / total_frames;
	average_update_time = (total_update_time * 1000) / total_frames;
	average_hotpath_memcpy_time = (total_hotpath_memcpy_time * 1000) / total_frames;
	printf("\n\n=======AVERAGES=======\nframe_time: %.5f ms\nupdate_time: %.5f ms\nhotpath_memcpy_time: %.5f ms\ntotal_frames: %lld\n\n",average_frame_time,average_update_time,average_hotpath_memcpy_time,total_frames);


	

free_and_exit:
	
	free_h_bodies(h_bodies);
	free_d_bodies(d_bodies, tmp_new_bufs);
	free(colors);

	if(WindowShouldClose())
	{
		CloseWindow();
		return 1;
	}

	CloseWindow();
	return 0;
}
