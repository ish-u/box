#include "raylib.h"
#include <math.h>

void sketch_orbit_b()
{
    ClearBackground(BLACK);
    int radius = 200;
    int x = radius * cos(GetTime() * 2);
    int y = -radius * sin(GetTime() * 2);
    x += GetScreenWidth() / 2;
    y += GetScreenHeight() / 2;
    DrawCircle(x, y, 20, WHITE);
}