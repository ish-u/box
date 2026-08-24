#include "raylib.h"
#include <time.h>
#include <stdio.h>

int IS_INIT = 0;
Font Inter;

void init()
{
    Inter = LoadFontEx("./assets/Inter.ttf", 280, 0, 95);
    SetTextureFilter(Inter.texture, TEXTURE_FILTER_BILINEAR);
}

void sketch_clock(void)
{
    if (!IS_INIT)
    {
        IS_INIT = 1;
        init();
    }
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char clock_str[9];
    snprintf(clock_str, sizeof(clock_str), "%02d:%02d:%02d",
             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    ClearBackground(WHITE);

    int font_size = GetScreenWidth() / 4;
    Vector2 text_size = MeasureTextEx(
        Inter,
        clock_str,
        font_size,
        2);

    float x = (GetScreenWidth() - text_size.x) / 2.0f;
    float y = (GetScreenHeight() - text_size.y) / 2.0f;

    DrawTextEx(Inter,
               clock_str,
               (Vector2){
                   x,
                   y,
               },
               font_size,
               2,
               BLACK);
}