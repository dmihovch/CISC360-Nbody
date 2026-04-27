#include "include/drawing.h"
#include "include/bodies.h"
#include "include/physics.h"
#include <cuda_runtime_api.h>
#include <raylib.h>
#include <cuda.h>

__global__ void dothething()
{
	
}


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

	Body* bodies = alloc_rand_nbodies(nbodies);
	if(bodies == NULL)
	{
		printf("Failed to allocate %d bodies\n",nbodies);
		return 1;
	}

	InitWindow(WIDTH, HEIGHT, "N-Body Simulation [ SERIAL ]");
	if(!IsWindowReady())
	{
		free(bodies);
		return 1;
	}
	
	dothething<<<1,1>>>();
	cudaDeviceSynchronize();
	printf("we did it joe!\n");
	return 69;

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
		update_bodies(bodies, nbodies);
		update_end = GetTime();


		render_start = GetTime();
		BeginDrawing();
		ClearBackground(BLACK);
		draw_bodies(bodies, nbodies);
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


	free(bodies);

	if(WindowShouldClose())
	{
		CloseWindow();
		return 1;
	}

	CloseWindow();
	return 0;
}
