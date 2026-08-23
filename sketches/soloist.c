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
    char album_name[256];
    char artist_name[512];
    char album_art[512];
    char playback_status[32];
    int total_duration;
    int current_duration;
    Texture2D album_art_texture;
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
            char *artist = strtok(NULL, "|");
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
            snprintf(CURRENT_STATE.artist_name,
                     sizeof(CURRENT_STATE.artist_name),
                     "%s", artist);
            snprintf(CURRENT_STATE.album_art,
                     sizeof(CURRENT_STATE.album_art),
                     "%s", art);
            snprintf(CURRENT_STATE.playback_status,
                     sizeof(CURRENT_STATE.playback_status),
                     "%s", status);
            CURRENT_STATE.total_duration = atoi(total);
            CURRENT_STATE.current_duration = atoi(current);

            if (strlen(CURRENT_STATE.album_art) > 0)
            {
                Image image = LoadImage(CURRENT_STATE.album_art);
                Texture2D texture = LoadTextureFromImage(image);
                CURRENT_STATE.album_art_texture = texture;
                UnloadImage(image);
            }
        }
    }
}

void truncate_text(char *text, int font_size, int max_width, char *truncated_text, int truncated_text_size)
{
    snprintf(truncated_text, truncated_text_size, "%s", text);
    if (MeasureText(truncated_text, font_size) > max_width)
    {
        int text_len = strlen(text);
        while (text_len > 0)
        {
            snprintf(truncated_text, truncated_text_size, "%.*s...", text_len, text);
            if (MeasureText(truncated_text, font_size) <= max_width)
            {
                return;
            }
            text_len--;
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

    int song_font_size = GetScreenWidth() / 16;
    int album_font_size = song_font_size / 4;
    int artist_font_size = song_font_size / 2;

    int spacing = song_font_size / 8;
    int margin = GetScreenWidth() / 16;
    int x = margin;
    int y = GetScreenHeight() - (song_font_size + album_font_size + artist_font_size + 4 * spacing);
    int max_allowed_width = GetScreenWidth() - 2 * margin;

    if (CURRENT_STATE.album_art_texture.id != 0)
    {
        Texture2D texture = CURRENT_STATE.album_art_texture;
        int album_art_x = x;
        int album_art_y = y - texture.height - 4 * spacing;
        DrawTexture(texture, album_art_x, album_art_y, WHITE);
    }

    char truncated_album_name[256];
    truncate_text(CURRENT_STATE.album_name,
                  song_font_size,
                  max_allowed_width,
                  truncated_album_name,
                  sizeof(truncated_album_name));
    DrawText(
        truncated_album_name,
        x,
        y,
        album_font_size,
        BLACK);
    y += album_font_size + spacing;

    char truncated_name[256];
    truncate_text(CURRENT_STATE.name,
                  song_font_size,
                  max_allowed_width,
                  truncated_name,
                  sizeof(truncated_name));
    DrawText(
        truncated_name,
        x,
        y,
        song_font_size,
        BLACK);
    y += song_font_size + spacing;

    char truncated_artist_name[512];
    truncate_text(CURRENT_STATE.artist_name,
                  song_font_size,
                  max_allowed_width,
                  truncated_artist_name,
                  sizeof(truncated_artist_name));
    DrawText(
        truncated_artist_name,
        x,
        y,
        artist_font_size,
        BLACK);
}
