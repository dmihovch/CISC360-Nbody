#ifndef DRAWING_H
#define DRAWING_H
#include "bodies.h"
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif  

void draw_bodies(Bodies b, Color* colors);
int draw_diagnostics(double frametime_start, double frametime_end, double render_start, double render_end, double update_start, double update_end, int nbodies);


#ifdef __cplusplus
	}
#endif
#endif //DRAWING_H
