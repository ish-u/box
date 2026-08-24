#include "raylib.h"
#include "sketches.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

int CURRENT_SKETCH = 0;

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
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_FULLSCREEN_MODE);
    const int monitor = 0;
    const int screenWidth = GetMonitorWidth(monitor) ? GetMonitorWidth(monitor) : 1920;
    const int screenHeight = GetMonitorHeight(monitor) ? GetMonitorHeight(monitor) : 1080;
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
            else
            {
                char *prefix = strtok(command, " ");
                char *value = strtok(NULL, " ");
                if (prefix && value && strncmp(prefix, "SKETCH", strlen("SKETCH")) == 0)
                {
                    int sketch_index = atoi(value);
                    if (sketch_index < TOTAL_SKETCHES && sketch_index >= 0)
                    {
                        CURRENT_SKETCH = sketch_index;
                    }
                }
            }
        }

        // Key Input
        if (IsKeyPressed(KEY_RIGHT))
        {
            CURRENT_SKETCH = (CURRENT_SKETCH + 1) % TOTAL_SKETCHES;
        }

        if (IsKeyPressed(KEY_LEFT))
        {
            CURRENT_SKETCH =
                (CURRENT_SKETCH - 1 + TOTAL_SKETCHES) % TOTAL_SKETCHES;
        }

        // Sketch Switch
        BeginDrawing();
        SKETCHES[CURRENT_SKETCH]();
        EndDrawing();
    }

    CloseWindow();
    close(pipe_fd);

    return 0;
}
