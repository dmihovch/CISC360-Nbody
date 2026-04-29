#include "include/drawing.h"
#include "include/bodies.h"
#include "include/physics.h"
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

	err = alloc_rand_nbodies_device(&d_bodies,nbodies);
	if(err)
	{
		printf("Failed to allocate %d bodies on GPU\n",nbodies);
		free_d_bodies(d_bodies);
		free_h_bodies(h_bodies);
		return 1;
	}


	InitWindow(WIDTH, HEIGHT, "N-Body Simulation [ SERIAL ]");
	if(!IsWindowReady())
	{
		free_h_bodies(h_bodies);
		free_d_bodies(d_bodies);
		return 1;
	}

	
	double hotpath_memcpy_start;
	double hotpath_memcpy_end;

	double frametime_start;
	double frametime_end;

	double render_start;
	double render_end;

	double update_start;
	double update_end;
	
	double total_frame_time = 0;
	double total_update_time = 0;
	long long total_frames = 0;

  //SetTargetFPS(FPS);
	while(!WindowShouldClose() && GetKeyPressed() != KEY_Q && GetTime() <= 10)
	{

		frametime_start = GetTime();

		update_start = GetTime();
		update_bodies(d_bodies);

		//errors
		cudaDeviceSynchronize();
		update_end = GetTime();

		hotpath_memcpy_start = GetTime();

		//errors
		cudaMemcpy(h_bodies.pos, d_bodies.pos, sizeof(Vector2)*nbodies, cudaMemcpyDeviceToHost);
		cudaMemcpy(h_bodies.vel, d_bodies.vel,sizeof(Vector2)* nbodies, cudaMemcpyDeviceToHost);
		cudaMemcpy(h_bodies.acc, d_bodies.acc, sizeof(Vector2)*nbodies, cudaMemcpyDeviceToHost);
		cudaMemcpy(h_bodies.m, d_bodies.m, sizeof(float)*nbodies, cudaMemcpyDeviceToHost);
		cudaMemcpy(h_bodies.r, d_bodies.r, sizeof(float)*nbodies, cudaMemcpyDeviceToHost);

		hotpath_memcpy_end = GetTime();

		render_start = GetTime();
		BeginDrawing();
		ClearBackground(BLACK);
		draw_bodies(h_bodies);
		render_end = GetTime();

		DrawFPS(0, 0);



		//draw_diagnostics(frametime_start, frametime_end, render_start, render_end, update_start, update_end, nbodies);

		EndDrawing();

		frametime_end = GetTime();

		total_frame_time += (frametime_end - frametime_start);
		total_update_time += (update_end - update_start);
		total_frames++;
	}


	double average_frame_time = (total_frame_time * 1000) / total_frames;
	double average_update_time = (total_update_time * 1000) / total_frames;
	printf("\n\n=======AVERAGES=======\nframe_time: %.5f ms\nupdate_time: %.5f ms\ntotal_frames: %lld\n\n",average_frame_time,average_update_time,total_frames);

	
	free_h_bodies(h_bodies);
	free_d_bodies(d_bodies);

	if(WindowShouldClose())
	{
		CloseWindow();
		return 1;
	}

	CloseWindow();
	return 0;
}
