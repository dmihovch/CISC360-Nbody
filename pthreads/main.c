#include "include/drawing.h"
#include "include/bodies.h"
#include "include/physics.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    srand(420);
    int nbodies = BODIES;
    int nthreads = 6;

    if(argc >= 2)
    {
        int argv_bodies = atoi(argv[1]);
        if(argv_bodies >= 1)
        {
            nbodies = argv_bodies;
        }
        else
        {
            printf("Your input couldn't be parsed, defaulting to %d bodies\n", nbodies);
        }
        
        if(argc >= 3)
        {
            int argv_threads = atoi(argv[2]);
            if(argv_threads >= 1)
            {
                nthreads = argv_threads;
            }
        }
    }
    else
    {
        printf("No body count passed to program\nUsing default value of %d bodies\nUsage: ./pthreads <body count> <thread count>\n", nbodies);
    }

    if(nthreads > nbodies)
    {
        nthreads = nbodies;
    }

    Bodies bodies;
    Color* colors;
    alloc_rand_nbodies(&bodies, &colors, nbodies);

    init_physics_threads(&bodies, nthreads);

    InitWindow(WIDTH, HEIGHT, "N-Body Simulation [ CPU THREADING ]");
    if(!IsWindowReady())
    {
        cleanup_physics_threads();
        free_bodies(&bodies, colors);
        return 1;
    }

    double frametime_start;
    double frametime_end;
    double render_start;
    double render_end;
    double update_start;
    double update_end;
    
    double total_frame_time = 0;
    double total_update_time = 0;
    double total_render_time = 0;
    long long total_frames = 0;

    while(!WindowShouldClose() && GetKeyPressed() != KEY_Q && GetTime() <= 30)
    {
        frametime_start = GetTime();

        update_start = GetTime();
        update_bodies(&bodies);
        update_end = GetTime();

        render_start = GetTime();
        BeginDrawing();
        ClearBackground(BLACK);
        draw_bodies(bodies, colors);
        DrawFPS(0, 0);
        EndDrawing();
        render_end = GetTime();

        frametime_end = GetTime();

        total_frame_time += (frametime_end - frametime_start);
        total_update_time += (update_end - update_start);
        total_render_time += (render_end - render_start);
        total_frames++;
    }

    double average_frame_time = (total_frame_time * 1000) / total_frames;
    double average_update_time = (total_update_time * 1000) / total_frames;
    double average_render_time = (total_render_time * 1000) / total_frames;
    printf("\n\n=======AVERAGES=======\nframe_time: %.5f ms\nupdate_time: %.5f ms\nrender_time: %.5f ms\ntotal_frames: %lld\n\n", average_frame_time, average_update_time, average_render_time, total_frames);
    
    cleanup_physics_threads();
    free_bodies(&bodies, colors);

    if(WindowShouldClose())
    {
        CloseWindow();
        return 1;
    }

    CloseWindow();
    return 0;
}

