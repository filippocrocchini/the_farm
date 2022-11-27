#include "raylib.h"

#include "game.h"

void GameState::render_frame()
{
    BeginMode2D(camera);

    for (int y = 0; y < level_size.y; y++)
    {
        for (int x = 0; x < level_size.x; x++)
        {
            const Tile& tile = tiles[y * level_size.x + x];
            
            Color color = LIME;
            if (tile.plowed)
            {
                color = BROWN;
            }

            DrawRectangle(x * TILE_SIZE_PIXELS, y * TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, color);
        }
    }

    for (const auto& e : tile_entities)
    {
        float age        = time_from_start - e.spawn_time;
        float age_factor = clamp(age / 2.f, 0.f, 1.f);

        Color color = color_mix(GREEN, YELLOW, age_factor);

        if (e.fully_grown) {
            color = RED;
        }

        DrawRectangle(e.tile_position.x * TILE_SIZE_PIXELS, e.tile_position.y * TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, color);
    }

    DrawRectangleLines(hovered_tile.x * TILE_SIZE_PIXELS, hovered_tile.y * TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, YELLOW);

    if (current_tool && tool_active)
    {
        DrawRectangleLines(last_pressed_tile.x * TILE_SIZE_PIXELS, last_pressed_tile.y * TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, ORANGE);
        
        Vector2 circle_center = last_pressed_tile * TILE_SIZE_PIXELS + Vector2i(TILE_SIZE_PIXELS, TILE_SIZE_PIXELS) / 2;
        DrawCircleSector(circle_center, TILE_SIZE_PIXELS / 4, 0, (1 - tool_time / current_tool->max_time) * 360, 20, WHITE);
    }

    for (const auto& e : entities)
    {
        DrawText(e.tool->name, e.position.x * UNIT_TO_PIXELS, e.position.y * UNIT_TO_PIXELS, 12, BLUE);
    }

    EndMode2D();
}