#include "game.h"

void Harvest::action(GameState* game_state)
{
    TileEntity* hovered_entity = nullptr;
    TileEntity* last_pressed_entity = nullptr;

    for (auto& e : game_state->tile_entities)
    {
        if (e.tile_position == game_state->hovered_tile) {
            hovered_entity = &e;
        }

        if (e.tile_position == game_state->last_pressed_tile) {
            last_pressed_entity = &e;
        }
    }
    
    if (last_pressed_entity && last_pressed_entity->fully_grown)
    {
        last_pressed_entity->dead = true;
    }

    if (game_state->last_pressed_tile.x >= 0 && game_state->last_pressed_tile.x < game_state->level_size.x &&
        game_state->last_pressed_tile.y >= 0 && game_state->last_pressed_tile.y < game_state->level_size.y)
    {
        Tile& tile = game_state->tiles[game_state->last_pressed_tile.x + game_state->last_pressed_tile.y * game_state->level_size.x];
        tile.plowed = false;
    }
}

void Plant::action(GameState* game_state)
{
    if (game_state->last_pressed_tile.x >= 0 && game_state->last_pressed_tile.x < game_state->level_size.x &&
        game_state->last_pressed_tile.y >= 0 && game_state->last_pressed_tile.y < game_state->level_size.y)
    {
        const Tile& tile = game_state->tiles[game_state->last_pressed_tile.x + game_state->last_pressed_tile.y * game_state->level_size.x];

        if (tile.plowed)
        {
            TileEntity e = {};
            e.tile_position = game_state->last_pressed_tile;
            e.spawn_time    = game_state->time_from_start;

            game_state->tile_entities.push_back(e);
        }
    }
}

void Plow::action(GameState* game_state)
{
    if (game_state->last_pressed_tile.x >= 0 && game_state->last_pressed_tile.x < game_state->level_size.x &&
        game_state->last_pressed_tile.y >= 0 && game_state->last_pressed_tile.y < game_state->level_size.y)
    {
        Tile& tile = game_state->tiles[game_state->last_pressed_tile.x + game_state->last_pressed_tile.y * game_state->level_size.x];
        tile.plowed = true;
    }
}

void GameState::init(Vector2i size)
{
    level_size = size;

    tiles.resize(level_size.x * level_size.y);

    for (Tile& tile : tiles)
    {
        tile.plowed = false;
    }

    entities.push_back(ToolEntity{ &HARVEST_TOOL, (Vector2)Vector2i { rand(), rand() } / (float)RAND_MAX * (Vector2)level_size });
    entities.push_back(ToolEntity{   &PLANT_TOOL, (Vector2)Vector2i { rand(), rand() } / (float)RAND_MAX * (Vector2)level_size });
    entities.push_back(ToolEntity{    &PLOW_TOOL, (Vector2)Vector2i { rand(), rand() } / (float)RAND_MAX * (Vector2)level_size });
}


void GameState::update(float delta)
{

    render_size = Vector2(GetScreenWidth(), GetScreenHeight());

    camera.target   = level_size / 2 * TILE_SIZE_PIXELS;
    camera.offset   = Vector2{ render_size.x / 2.0f, render_size.y / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom     = 2.0f;

    time_from_start += delta;

    auto mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera) / (float) UNIT_TO_PIXELS;

    hovered_tile.x = floor(mouse_pos.x / TILE_SIZE_UNITS);
    hovered_tile.y = floor(mouse_pos.y / TILE_SIZE_UNITS);

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        if (current_tool_entity) {
            current_tool_entity = nullptr;
            current_tool = nullptr;
        }
        else {
            for (auto& e : entities)
            {
                if (length(e.position - mouse_pos) < 0.5f)
                {
                    current_tool_entity = &e;
                    current_tool = e.tool;
                    break;
                }
            }
        }
    }

    if (current_tool_entity)
    {
        current_tool_entity->position = mouse_pos;
    }

    if (current_tool)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            last_pressed_tile = hovered_tile;
            tool_time         = current_tool->max_time;
            tool_active       = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            tool_active = 0;
        }
    }

    for (auto& e : tile_entities)
    {
        float age = time_from_start - e.spawn_time;

        if (age > 2) e.fully_grown = true;
    }

    if (current_tool && tool_active)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            tool_time = fc_max(tool_time - delta, 0.f);
        
            if (tool_time <= 0)
            {
                current_tool->action(this);
                tool_active = false;
            }
        }
    }

    std::erase_if(tile_entities, [](auto e) { return e.dead; });
}