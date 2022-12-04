#include "raylib.h"

#include "game.h"

// #include <string>

void GameState::draw_sprite(Sprite sprite, Vector2 position, bool flip_x, bool flip_y)
{
    Texture2D texture = textures[sprite.texture];
    
    Rectangle source = (Rectangle) sprite.region;

    source.x      += 0.125f;
    source.y      += 0.125f;
    source.width  -= 0.25f;
    source.height -= 0.25f;

    if (flip_x) source.width = -source.width;
    if (flip_y) source.height = -source.height;

    DrawTexturePro(texture, source, sprite.region + position, sprite.region.min + sprite.origin, 0, WHITE);
}

void GameState::render_frame()
{
    BeginMode2D(camera);

    for (int y = 0; y < level_size.y; y++)
    {
        for (int x = 0; x < level_size.x; x++)
        {
            const auto& tile   = tiles[y * level_size.x + x];
            const auto& info   = tile_infos[tile.info];
            const auto& sprite = sprites[info.sprite];

            draw_sprite(sprite, Vector2{ (float)x * TILE_SIZE_PIXELS, (float)y * TILE_SIZE_PIXELS }, tile.random & 1, (tile.random >> 1) & 1);
        }
    }

    for (const auto& e : tile_entities)
    {
        const auto& info = tile_entity_infos[e.info];

        float age        = time_from_start - e.spawn_time;
        float age_factor = clamp(age / 2.f, 0.f, 1.f);

        size_t sprite_index = (int) (age_factor * (info.sprites.size() - 1));

        const auto& sprite = sprites[info.sprites[sprite_index]];

        draw_sprite(sprite, e.tile_position * UNIT_TO_PIXELS);
    }

    Entity* current_entity = get_entity(current_entity_id);

    if (current_entity)
    {
        const auto& info = entity_infos[current_entity->info];
        const auto& tool = tools[info.tool];
        
        DrawRectangleLines(hovered_tile.x * TILE_SIZE_PIXELS, hovered_tile.y * TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, YELLOW);

        if (tool->max_time && tool_active)
        {
            DrawRectangleLines(last_pressed_tile.x * TILE_SIZE_PIXELS, last_pressed_tile.y * TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, TILE_SIZE_PIXELS, ORANGE);
        
            Vector2 circle_center = last_pressed_tile * TILE_SIZE_PIXELS + Vector2i(TILE_SIZE_PIXELS, TILE_SIZE_PIXELS) / 2;
            DrawCircleSector(circle_center, TILE_SIZE_PIXELS / 4, 0, (1 - tool_time / tool->max_time) * 360, 20, WHITE);
        }
    }

    for (const auto& e : entities)
    {
        const auto& info   = entity_infos[e.second.info];
        const auto& sprite = sprites[info.sprite];

        draw_sprite(sprite, e.second.position * UNIT_TO_PIXELS);
    }

    EndMode2D();
    
    // DrawText(std::to_string(current_entity_id).c_str(), 10, 10, 24, RED);
}