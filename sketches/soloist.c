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
            char *name = strtok(command, "|");
            char *album = strtok(NULL, "|");
            char *art = strtok(NULL, "|");
            char *status = strtok(NULL, "|");
            char *total = strtok(NULL, "|");
            char *current = strtok(NULL, "|");
            if (!name || !album || !art || !status || !total || !current)
                return;

            snprintf(CURRENT_STATE.name,
                     sizeof(CURRENT_STATE.name),
                     "%s", name);
            snprintf(CURRENT_STATE.album_name,
                     sizeof(CURRENT_STATE.album_name),
                     "%s", album);
            snprintf(CURRENT_STATE.album_art,
                     sizeof(CURRENT_STATE.album_art),
                     "%s", art);
            snprintf(CURRENT_STATE.playback_status,
                     sizeof(CURRENT_STATE.playback_status),
                     "%s", status);
            CURRENT_STATE.total_duration = atoi(total);
            CURRENT_STATE.current_duration = atoi(current);
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

    int song_width = MeasureText(CURRENT_STATE.name, font_size);
    int album_font_size = font_size / 2;
    int album_width = MeasureText(CURRENT_STATE.album_name, album_font_size);

    int song_x = GetScreenWidth() / 2 - song_width / 2;
    int album_x = GetScreenWidth() / 2 - album_width / 2;

    int y = GetScreenHeight() / 2 - font_size;

    DrawText(
        CURRENT_STATE.name,
        song_x,
        y,
        font_size,
        BLACK);

    y += font_size;

    DrawText(
        CURRENT_STATE.album_name,
        album_x,
        y,
        album_font_size,
        BLACK);
}
