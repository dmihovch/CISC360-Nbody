#include "include/drawing.h"
#include "include/bodies.h"
#include "include/physics.h"
#include <raylib.h>


int main(int argc, char** argv){
	InitWindow(WIDTH, HEIGHT, "N-Body Simulation [ SERIAL ]");
	if(!IsWindowReady())
	{
		return 1;
	}

	srand(time(NULL));
	

	int nbodies = BODIES;

	if(argc >= 2)
	{
		int argv_bodies = atoi(argv[1]);
		if(argv_bodies >= 1)
		{
			nbodies = argv_bodies;
		}
		else{
			printf("Your input resulted in nbodies == 0, defaulting to %d\n",nbodies);
		}
	}
	else{
		printf("No particle count passed to program\nUsing default value of %d\nUsage: ./serial <body count>\n",nbodies);
	}

	Body* bodies = alloc_rand_nbodies(nbodies);
	if(bodies == NULL)
	{
		CloseWindow();
		return 1;
	}
	

	double frametime_start;
	double frametime_end;

	double render_start;
	double render_end;

	double update_start;
	double update_end;
	
	SetTargetFPS(FPS);
	while(!WindowShouldClose() && GetKeyPressed() != KEY_Q)
	{

		frametime_start = GetTime();

		update_start = GetTime();
		update_bodies(bodies, nbodies);
		update_end = GetTime();


		render_start = GetTime();
		BeginDrawing();
		ClearBackground(GRAY);
		draw_bodies(bodies, nbodies);
		render_end = GetTime();

		DrawFPS(0, 0);

		frametime_end = GetTime();

		draw_diagnostics(frametime_start, frametime_end, render_start, render_end, update_start, update_end, nbodies);

		EndDrawing();

	}

	free(bodies);

	if(WindowShouldClose())
	{
		CloseWindow();
		return 1;
	}

	CloseWindow();
	return 0;
}
