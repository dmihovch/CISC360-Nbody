#include "../include/drawing.h"
void draw_bodies(Bodies b_arr, Color* colors)
{
	for(int i = 0; i<b_arr.nbodies; ++i)
	{
		DrawCircle(b_arr.pos[i].x, b_arr.pos[i].y,b_arr.r[i], colors[i]);
	}
	return;
}


int draw_diagnostics(double frametime_start, double frametime_end, double render_start, double render_end, double update_start, double update_end, int nbodies)
{
	double tft = 1000.0 * (frametime_end - frametime_start);
	double trt = 1000.0 * (render_end - render_start);
	double tut = 1000.0 * (update_end - update_start);
	double over = tft - trt - tut;

	int x_pos = (int)(WIDTH * 0.01f);
	int y_pos = (int)(HEIGHT * 0.04f);

	DrawText(TextFormat("total frametime: %.3f ms", tft), x_pos, y_pos, 22, WHITE);
	DrawText(TextFormat("render time: %.3f ms", trt), x_pos, y_pos + 30, 22, WHITE);
	DrawText(TextFormat("update time: %.3f ms", tut), x_pos, y_pos + 60, 22, WHITE);
	DrawText(TextFormat("overhead time: %.3f ms", over), x_pos, y_pos + 90, 22, WHITE);
	DrawText(TextFormat("bodies: %d", nbodies), x_pos, y_pos + 120, 22, WHITE);

	return 0;
}
