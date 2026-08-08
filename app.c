#include "raylib.h"
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

int CURRENT_SKETCH = 0;
#define TOTAL_SKETCHES 2

void SKETCH_1()
{
    ClearBackground(WHITE);
    int radius = 200;
    int x = radius * cos(GetTime() * 2);
    int y = -radius * sin(GetTime() * 2);
    x += GetScreenWidth() / 2;
    y += GetScreenHeight() / 2;
    DrawCircle(x, y, 20, BLACK);
}

void SKETCH_2()
{
    ClearBackground(BLACK);
    int radius = 200;
    int x = radius * cos(GetTime() * 2);
    int y = -radius * sin(GetTime() * 2);
    x += GetScreenWidth() / 2;
    y += GetScreenHeight() / 2;
    DrawCircle(x, y, 20, WHITE);
}

int main(void)
{

    // Pipe Setup
    if (mkfifo("/tmp/pipe", 0666) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        return 1;
    }
    char command[128];
    int pipe_fd;
    pipe_fd = open("/tmp/pipe", O_RDONLY | O_NONBLOCK);

    // Raylib Initial Config
    const int monitor = 0;
    const int screenWidth = GetMonitorWidth(monitor);
    const int screenHeight = GetMonitorHeight(monitor);
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_FULLSCREEN_MODE);
    InitWindow(screenWidth, screenHeight, "init");
    SetTargetFPS(60);

    // Raylib Event Loop
    while (!WindowShouldClose())
    {

        // Command Processing
        ssize_t n = read(pipe_fd, command, sizeof(command) - 1);
        if (n > 0)
        {
            command[n] = '\0';
            printf("command: %s", command);
            if (strcmp(command, "NEXT\n") == 0)
            {
                CURRENT_SKETCH = (CURRENT_SKETCH + 1) % TOTAL_SKETCHES;
            }
            else if (strcmp(command, "PREV\n") == 0)
            {
                CURRENT_SKETCH =
                    (CURRENT_SKETCH - 1 + TOTAL_SKETCHES) % TOTAL_SKETCHES;
            }
        }

        // Sketch Switch
        BeginDrawing();
        switch (CURRENT_SKETCH)
        {
        case 0:
            SKETCH_1();
            break;
        case 1:
            SKETCH_2();
            break;
        default:
            break;
        }
        EndDrawing();
    }

    CloseWindow();
    close(pipe_fd);

    return 0;
}
