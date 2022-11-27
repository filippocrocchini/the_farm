#include "raylib.h"
#include "raymath.h"

#include <string>

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int scale = 4;

    InitWindow(screenWidth, screenHeight, "The Farm");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    
    SetExitKey(0);

    double last_frame_time = 0;

    while (!WindowShouldClose())
    {
        auto frame_start = GetTime();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        auto window_width  = GetRenderWidth();
        auto window_height = GetRenderHeight();

        std::string text = "Congrats! You created your first window!";
        text += " (" + std::to_string((int) (1 / last_frame_time)) + ")";

        auto text_size = MeasureText(text.c_str(), GetFontDefault().baseSize * scale);

        DrawText(text.c_str(), (window_width - text_size) / 2, (window_height - GetFontDefault().baseSize * scale) / 2, GetFontDefault().baseSize * scale, LIGHTGRAY);

        EndDrawing();

        auto frame_end = GetTime();
        last_frame_time = Lerp(last_frame_time, frame_end - frame_start, 0.0125f);
    }

    CloseWindow();

    return 0;
}