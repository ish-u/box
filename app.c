#include "raylib.h"
#include <math.h>

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight, "init");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        int radius = 100;
        int x = radius * cos(GetTime() * 2);
        int y = -radius * sin(GetTime() * 2);
        x += GetScreenWidth()/2;
        y += GetScreenHeight()/2;
        DrawCircle(x,y, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
