#include "raylib.h"
#include <time.h>
#include <stdio.h>

void sketch_clock(void)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char clock_str[9];
    snprintf(clock_str, sizeof(clock_str), "%02d:%02d:%02d",
             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    ClearBackground(WHITE);

    int font_size = GetScreenWidth() / 4;
    int text_width = MeasureText(clock_str, font_size);

    int x = GetScreenWidth() / 2 - text_width / 2;
    int y = GetScreenHeight() / 2 - font_size / 2;

    DrawText(clock_str, x, y, font_size, BLACK);
}