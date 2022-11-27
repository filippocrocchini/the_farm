#include "raylib.h"
#include "game.h"

int main(void)
{
    Vector2 last_window_size = {800, 450};
    InitWindow(last_window_size.x, last_window_size.y, "The Farm");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    SetExitKey(0);

    GameState game_state;

    game_state.init(Vector2i(8, 8));

    double last_frame_time = 0;
    while (!WindowShouldClose())
    {
        auto frame_start = GetTime();

        if (IsKeyPressed(KEY_F11))
        {
            bool was_fullscreen = IsWindowFullscreen();

            if(!was_fullscreen)
            {
                last_window_size.x = GetScreenWidth();
                last_window_size.y = GetScreenHeight();

                int display = GetCurrentMonitor();
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
            }

            ToggleFullscreen();

            if (was_fullscreen)
            {
                SetWindowSize(last_window_size.x, last_window_size.y);
            }
        }

        game_state.update(last_frame_time);

        BeginDrawing();

        ClearBackground(DARKGRAY);

        game_state.render_frame();

        EndDrawing();

        auto frame_end = GetTime();
        last_frame_time = lerp(last_frame_time, frame_end - frame_start, 0.0125f);
    }

    CloseWindow();

    return 0;
}