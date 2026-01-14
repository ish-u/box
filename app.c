
#include "raylib.h"

int main(void)
{
    const int screenWidth = 400;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "init");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(WHITE);
        DrawText("INIT", 190, 200, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
