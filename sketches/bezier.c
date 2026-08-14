#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>

void sketch_bezier()
{
    ClearBackground(BLACK);

    float a = 100.0f;
    float t = GetTime() * 2.0f;

    float x = a * cosf(t);
    float y = -a * sinf(t);

    for (int i = 0; i < GetScreenWidth(); i += a)
    {
        for (int j = 0; j < GetScreenHeight(); j += a)
        {

            float dir = (i + j) % (int)(2 * a) == 0 ? -1.0f : 1.0f;
            Vector2 points[] = {
                {i, j},
                {i, j + y * dir},
                {i + x * dir, j},
                {i + a / 2, j + a / 2},
            };

            DrawSplineBezierCubic(points, 4, 2.0f, WHITE);
        }
    }
}