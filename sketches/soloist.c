#include "raylib.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include "raylib.h"
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

int SOLOIST_PIPE = 0;
struct
{
    char name[256];
    // TODO -
    char album_name[256];
    char album_art[512];
    char playback_status[32];
    int total_duration;
    int current_duration;
} CURRENT_STATE = {0};

void init_soloist_pipe()
{
    printf("SOLOIST_INIT\n");
    if (mkfifo("/tmp/soloist", 0666) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        return;
    }
    SOLOIST_PIPE = open("/tmp/soloist", O_RDONLY | O_NONBLOCK);
}

void read_soloist_pipe()
{
    if (SOLOIST_PIPE > 0)
    {
        char command[2048];
        ssize_t n = read(SOLOIST_PIPE, command, sizeof(command) - 1);
        if (n > 0)
        {
            command[n] = '\0';
            strcpy(CURRENT_STATE.name, command);
            printf("%s", CURRENT_STATE.name);
        }
    }
}

void sketch_soloist()
{
    if (SOLOIST_PIPE == 0)
    {
        init_soloist_pipe();
    }
    if (SOLOIST_PIPE > 0)
    {
        read_soloist_pipe();
    }
    ClearBackground(WHITE);

    int font_size = GetScreenWidth() / 16;
    int text_width = MeasureText(CURRENT_STATE.name, font_size);

    int x = GetScreenWidth() / 2 - text_width / 2;
    int y = GetScreenHeight() / 2 - font_size / 2;

    DrawText(CURRENT_STATE.name, x, y, font_size, BLACK);
}
