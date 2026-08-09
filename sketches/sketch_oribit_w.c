#include "raylib.h"
#include <math.h>

void sketch_orbit_w()
{
    ClearBackground(WHITE);
    int radius = 200;
    int x = radius * cos(GetTime() * 2);
    int y = -radius * sin(GetTime() * 2);
    x += GetScreenWidth() / 2;
    y += GetScreenHeight() / 2;
    DrawCircle(x, y, 20, BLACK);
}