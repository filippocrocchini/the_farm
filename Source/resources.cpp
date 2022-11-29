#include "game.h"

#include "raylib.h"

static auto HARVEST_TOOL = Harvest{ 0.2f };
static auto PLANT_TOOL   = Plant{ 0.2f };
static auto PLOW_TOOL    = Plow{ 0.2f };
static auto NULL_TOOL    = NoTool{};

entity_info_id GameState::register_entity_info(EntityInfo info)
{
    entity_infos.push_back(info);
    return entity_infos.size() - 1;
}

tile_entity_info_id GameState::register_tile_entity_info(TileEntityInfo info)
{
    tile_entity_infos.push_back(info);
    return tile_entity_infos.size() - 1;
}

texture_id GameState::register_texture(Texture2D texture)
{
    textures.push_back(texture);
    return textures.size() - 1;
}

tile_info_id GameState::register_tile_info(TileInfo info)
{
    tile_infos.push_back(info);
    return tile_infos.size() - 1;
}

sprite_id GameState::register_sprite(Sprite sprite)
{
    sprites.push_back(sprite);
    return sprites.size() - 1;
}

tool_id GameState::register_tool(Tool* tool)
{
    tools.push_back(tool);
    return tools.size() - 1;
}

void GameState::load_resources()
{
    Image null_image = GenImageColor(1, 1, MAGENTA);
    null_texture = register_texture(LoadTextureFromImage(null_image));

    main_atlas = register_texture(LoadTexture("main_atlas.png"));
    
    null_sprite = register_sprite(Sprite{ null_texture, Rectangle2i::make_min_size(Vector2i{  0, 0 }, Vector2i{ 1, 1 }), Vector2{} });

    harvest_sprite = register_sprite(Sprite{ main_atlas, Rectangle2i::make_min_size(Vector2i{  0, 0 }, Vector2i{ 16, 16 }), Vector2{} });
    plow_sprite    = register_sprite(Sprite{ main_atlas, Rectangle2i::make_min_size(Vector2i{ 16, 0 }, Vector2i{ 16, 16 }), Vector2{} });
    seed_sprite    = register_sprite(Sprite{ main_atlas, Rectangle2i::make_min_size(Vector2i{ 32, 0 }, Vector2i{ 16, 16 }), Vector2{} });
    plant_sprite   = register_sprite(Sprite{ main_atlas, Rectangle2i::make_min_size(Vector2i{ 48, 0 }, Vector2i{ 16, 16 }), Vector2{} });
    
    wheat_sprite_0 = register_sprite(Sprite{ main_atlas, Rectangle2i::make_min_size(Vector2i{  0, 16 }, Vector2i{ 16, 16 }), Vector2{} });
    wheat_sprite_1 = register_sprite(Sprite{ main_atlas, Rectangle2i::make_min_size(Vector2i{ 16, 16 }, Vector2i{ 16, 16 }), Vector2{} });
    wheat_sprite_2 = register_sprite(Sprite{ main_atlas, Rectangle2i::make_min_size(Vector2i{ 32, 16 }, Vector2i{ 16, 16 }), Vector2{} });
    wheat_sprite_3 = register_sprite(Sprite{ main_atlas, Rectangle2i::make_min_size(Vector2i{ 48, 16 }, Vector2i{ 16, 16 }), Vector2{} });

    grass_sprite       = register_sprite(Sprite{ main_atlas, Rectangle2i::make_min_size(Vector2i{  0, 32 }, Vector2i{ 16, 16 }), Vector2{} });
    dirt_sprite        = register_sprite(Sprite{ main_atlas, Rectangle2i::make_min_size(Vector2i{ 16, 32 }, Vector2i{ 16, 16 }), Vector2{} });
    plowed_dirt_sprite = register_sprite(Sprite{ main_atlas, Rectangle2i::make_min_size(Vector2i{ 32, 32 }, Vector2i{ 16, 16 }), Vector2{} });

    null_tool    = register_tool(&NULL_TOOL);
    harvest_tool = register_tool(&HARVEST_TOOL);
    plow_tool    = register_tool(&PLOW_TOOL);
    plant_tool   = register_tool(&PLANT_TOOL);

    null_entity_info  = register_entity_info(EntityInfo{ null_sprite, null_tool });
    harvest_tool_info = register_entity_info(EntityInfo{ harvest_sprite, harvest_tool });
    plow_tool_info    = register_entity_info(EntityInfo{ plow_sprite, plow_tool });
    seed_info         = register_entity_info(EntityInfo{ seed_sprite, plant_tool });
    plant_info        = register_entity_info(EntityInfo{ plant_sprite , 0 });

    null_tile_entity_info = register_tile_entity_info(TileEntityInfo{ { null_sprite } });
    wheat_info            = register_tile_entity_info(TileEntityInfo{ { wheat_sprite_0, wheat_sprite_1, wheat_sprite_2, wheat_sprite_3 } });

    null_tile_info   = register_tile_info(TileInfo{ null_sprite });
    grass_info       = register_tile_info(TileInfo{ grass_sprite, false});
    dirt_info        = register_tile_info(TileInfo{ dirt_sprite, false });
    plowed_dirt_info = register_tile_info(TileInfo{ plowed_dirt_sprite, true });


    UnloadImage(null_image);
}