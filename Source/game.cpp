#include "game.h"

bool Harvest::can_act(GameState* game_state)
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

    return last_pressed_entity && last_pressed_entity->fully_grown;
}

void Harvest::action(GameState* game_state)
{
    TileEntity* last_pressed_entity = nullptr;

    for (auto& e : game_state->tile_entities)
    {
        if (e.tile_position == game_state->last_pressed_tile) {
            last_pressed_entity = &e;
        }
    }
    
    if (last_pressed_entity && last_pressed_entity->fully_grown)
    {
        game_state->add_entity(Entity{ game_state->seed_info, (Vector2)Vector2i { rand(), rand() } / (float)RAND_MAX * (Vector2)game_state->level_size, 1 });
        game_state->add_entity(Entity{ game_state->plant_info, (Vector2)Vector2i { rand(), rand() } / (float)RAND_MAX * (Vector2)game_state->level_size, 1 });
        
        last_pressed_entity->dead = true;

        if (game_state->last_pressed_tile.x >= 0 && game_state->last_pressed_tile.x < game_state->level_size.x &&
            game_state->last_pressed_tile.y >= 0 && game_state->last_pressed_tile.y < game_state->level_size.y)
        {
            Tile& tile = game_state->tiles[game_state->last_pressed_tile.x + game_state->last_pressed_tile.y * game_state->level_size.x];
            tile.info = game_state->dirt_info;
        }
    }
}

bool Plant::can_act(GameState* game_state)
{
    if (game_state->last_pressed_tile.x >= 0 && game_state->last_pressed_tile.x < game_state->level_size.x &&
        game_state->last_pressed_tile.y >= 0 && game_state->last_pressed_tile.y < game_state->level_size.y)
    {
        const auto& tile = game_state->tiles[game_state->last_pressed_tile.x + game_state->last_pressed_tile.y * game_state->level_size.x];
        const auto& info = game_state->tile_infos[tile.info];

        return info.can_plant;
    }

    return false;
}

void Plant::action(GameState* game_state)
{
    TileEntity e = {};
    e.tile_position = game_state->last_pressed_tile;
    e.spawn_time    = game_state->time_from_start;
    e.info          = game_state->wheat_info;
    e.random        = (size_t)GetRandomValue(INT_MIN, INT_MAX) << 32 | GetRandomValue(INT_MIN, INT_MAX);


    game_state->tile_entities.push_back(e);
}

bool Plow::can_act(GameState* game_state)
{
    if (game_state->last_pressed_tile.x >= 0 && game_state->last_pressed_tile.x < game_state->level_size.x &&
        game_state->last_pressed_tile.y >= 0 && game_state->last_pressed_tile.y < game_state->level_size.y)
    {
        const auto& tile = game_state->tiles[game_state->last_pressed_tile.x + game_state->last_pressed_tile.y * game_state->level_size.x];
        const auto& info = game_state->tile_infos[tile.info];

        return !info.can_plant;
    }

    return false;
}

void Plow::action(GameState* game_state)
{
    if (game_state->last_pressed_tile.x >= 0 && game_state->last_pressed_tile.x < game_state->level_size.x &&
        game_state->last_pressed_tile.y >= 0 && game_state->last_pressed_tile.y < game_state->level_size.y)
    {
        Tile& tile = game_state->tiles[game_state->last_pressed_tile.x + game_state->last_pressed_tile.y * game_state->level_size.x];
        tile.info = game_state->plowed_dirt_info;
    }
}


bool GameState::collides_with(Entity* entity, Vector2 point)
{
    const auto& info   = entity_infos[entity->info];
    const auto& sprite = sprites[info.sprite];

    Rectangle2 entity_region = Rectangle2::make_min_size(entity->position, (Vector2)(sprite.region.max - sprite.region.min) / (float)UNIT_TO_PIXELS); 

    return contains(entity_region, point);
}

void GameState::init(Vector2i size)
{
    level_size = size;

    tiles.resize(level_size.x * level_size.y);

    for (Tile& tile : tiles)
    {
        tile.info   = grass_info;
        tile.random = (size_t)GetRandomValue(INT_MIN, INT_MAX) << 32 | GetRandomValue(INT_MIN, INT_MAX);
    }

    add_entity(Entity{ harvest_tool_info, (Vector2)Vector2i { rand(), rand() } / (float)RAND_MAX * (Vector2)level_size, INT_MAX });
    add_entity(Entity{ plow_tool_info, (Vector2)Vector2i { rand(), rand() } / (float)RAND_MAX * (Vector2)level_size, INT_MAX });

    add_entity(Entity{ seed_info, (Vector2)Vector2i { rand(), rand() } / (float)RAND_MAX * (Vector2)level_size, 1});
}

void GameState::add_entity(const Entity& e)
{
    last_id += 1;
    auto entity = entities.insert({ last_id, e });
    
    entity.first->second.id = last_id; // This is nuts
}

void GameState::remove_entity(int id)
{
    entities.erase(id);
}

Entity* GameState::get_entity(int id)
{
    auto entity_itr = entities.find(id);
    if (entity_itr != entities.end())
    {
        return &entity_itr->second;
    }
    return nullptr;
}

void GameState::update(float delta)
{

    render_size = Vector2i(GetScreenWidth(), GetScreenHeight());

    camera.target   = level_size / 2 * TILE_SIZE_PIXELS;
    camera.offset   = Vector2{ render_size.x / 2.0f, render_size.y / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom     = 2.0f;

    time_from_start += delta;

    auto mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera) / (float) UNIT_TO_PIXELS;

    hovered_tile.x = floor(mouse_pos.x / TILE_SIZE_UNITS);
    hovered_tile.y = floor(mouse_pos.y / TILE_SIZE_UNITS);

    bool just_picked_up = false;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (!current_entity_id) {
            for (auto& e : entities)
            {
                if (collides_with(&e.second, mouse_pos))
                {
                    current_entity_id = e.first;
                    just_picked_up = true;
                    break;
                }
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        if (current_entity_id) {
            current_entity_id = 0;
        }
    }

    for (auto& e : tile_entities)
    {
        float age = time_from_start - e.spawn_time;

        if (age > 2) e.fully_grown = true;
    }

    Entity* current_entity = get_entity(current_entity_id);

    if (current_entity)
    {
        current_entity->position = mouse_pos;

        const auto& info = entity_infos[current_entity->info];
        const auto& tool = tools[info.tool];

        if (!just_picked_up && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            last_pressed_tile = hovered_tile;
            tool_time         = tool->max_time;
            tool_active       = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            tool_active = false;
        }

        if (tool_active)
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                tool_time = fc_max(tool_time - delta, 0.f);
        
                if (tool_time <= 0 && tool->can_act(this))
                {
                    tool->action(this);
                    tool_active = false;
                    
                    if (current_entity->count != INT_MAX)
                    {
                        current_entity->count -= 1;
                    }

                    if (current_entity->count <= 0)
                    {
                        remove_entity(current_entity_id);
                        current_entity_id = 0;
                    }
                }
            }
        }
    }

    std::erase_if(tile_entities, [](auto e) { return e.dead; });
}