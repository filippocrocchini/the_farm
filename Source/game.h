#pragma once

#include "my_math.h"

#include <vector>
#include <unordered_map>

constexpr int UNIT_TO_PIXELS = 16;

constexpr int TILE_SIZE_UNITS  = 1;
constexpr int TILE_SIZE_PIXELS = TILE_SIZE_UNITS * UNIT_TO_PIXELS;
constexpr int PIXEL_SCALE      = 3;

typedef size_t entity_info_id;
typedef size_t tile_entity_info_id;
typedef size_t tile_info_id;
typedef size_t tool_id;
typedef size_t entity_id;
typedef size_t texture_id;
typedef size_t sprite_id;

struct GameState;

struct Tool
{
    float max_time = 0;
    virtual void action(GameState* game_state)  = 0;
    virtual bool can_act(GameState* game_state) = 0;

    constexpr explicit Tool(float max_time) : max_time { max_time } {}

    constexpr Tool() = default;
    constexpr Tool(const Tool&) = default;
    constexpr Tool(Tool&&) = default;
    constexpr Tool& operator=(const Tool&) = default;
    constexpr Tool& operator=(Tool&&) = default;

    virtual ~Tool() = default;
};

struct NoTool : Tool {
    constexpr NoTool() : Tool{ 0 } {}
    inline void action(GameState* game_state) override {}
    inline bool can_act(GameState* game_state) override { return false; }
};

struct Harvest : Tool {
    constexpr Harvest(float max_time) : Tool{ max_time } {}
    void action(GameState* game_state) override;
    bool can_act(GameState* game_state) override;
};

struct Plant : Tool {
    constexpr Plant(float max_time) : Tool{ max_time } {}
    void action(GameState* game_state) override;
    bool can_act(GameState* game_state) override;
};

struct Plow : Tool {
    constexpr Plow(float max_time) : Tool{ max_time } {}
    void action(GameState* game_state) override;
    bool can_act(GameState* game_state) override;
};

struct Sprite
{
    texture_id  texture = 0;
    Rectangle2i region = {};
    Vector2     origin = {};

    constexpr Sprite(texture_id texture, Rectangle2i region, Vector2 origin) : texture{ texture }, region{ region }, origin{ origin } {}

    constexpr Sprite() = default;
    constexpr Sprite(const Sprite&) = default;
    constexpr Sprite(Sprite&&) = default;
    constexpr Sprite& operator=(const Sprite&) = default;
    constexpr Sprite& operator=(Sprite&&) = default;
};

struct TileInfo
{
    sprite_id sprite = 0;
    bool   can_plant = false;
};

struct Tile
{
    tile_info_id info = 0;
    size_t     random = 0;
};

struct TileEntityInfo
{
    std::vector<sprite_id> sprites = {};
};

struct TileEntity
{
    tile_entity_info_id info = 0;

    size_t          random = 0;
    Vector2i tile_position = {};
    bool       fully_grown = false;
    bool              dead = false;
    float       spawn_time = 0;
};

struct EntityInfo
{
    sprite_id sprite = 0;
    tool_id     tool = 0;
};

struct Entity
{
    entity_info_id info = 0;
   
    Vector2 position = {};
    int        count = 1;
    entity_id     id = 0;

    constexpr Entity(entity_info_id info, Vector2 position, int count) : info{ info }, position{ position }, count{ count } {}
    constexpr Entity() = default;
};

struct GameState
{
    Camera2D       camera = {};
    float time_from_start = 0;
    Vector2i  render_size = {};

    //  ###################
    //  #    RESOURCES    #
    //  ###################
    std::vector<TileEntityInfo> tile_entity_infos;
    std::vector<EntityInfo>     entity_infos;
    std::vector<Texture2D>      textures;
    std::vector<TileInfo>       tile_infos;
    std::vector<Sprite>         sprites;
    std::vector<Tool*>          tools;

    tile_entity_info_id register_tile_entity_info(TileEntityInfo info);
    entity_info_id      register_entity_info(EntityInfo info);
    tile_info_id        register_tile_info(TileInfo info);
    texture_id          register_texture(Texture2D texture);
    sprite_id           register_sprite(Sprite sprite);
    tool_id             register_tool(Tool* tool);

    texture_id null_texture = 0;
    texture_id   main_atlas = 0;

    sprite_id    null_sprite = 0;
    sprite_id harvest_sprite = 0;
    sprite_id    plow_sprite = 0;
    sprite_id    seed_sprite = 0;
    sprite_id   plant_sprite = 0;

    sprite_id wheat_sprite_0 = 0;
    sprite_id wheat_sprite_1 = 0;
    sprite_id wheat_sprite_2 = 0;
    sprite_id wheat_sprite_3 = 0;

    sprite_id bin_sprite = 0;
    
    sprite_id       grass_sprite = 0;
    sprite_id plowed_dirt_sprite = 0;
    sprite_id        dirt_sprite = 0;

    tool_id    null_tool = 0;
    tool_id harvest_tool = 0;
    tool_id   plant_tool = 0;
    tool_id    plow_tool = 0;

    entity_info_id  null_entity_info = 0;
    entity_info_id harvest_tool_info = 0;
    entity_info_id    plow_tool_info = 0;
    entity_info_id         seed_info = 0;
    entity_info_id        plant_info = 0;
    entity_info_id          bin_info = 0;

    tile_entity_info_id null_tile_entity_info = 0;
    tile_entity_info_id            wheat_info = 0;

    tile_info_id   null_tile_info = 0;
    tile_info_id       grass_info = 0;
    tile_info_id        dirt_info = 0;
    tile_info_id plowed_dirt_info = 0;

    //  ###############
    //  #    TILES    #
    //  ###############
    Vector2i     level_size = {};
    std::vector<Tile> tiles = {};

    //  ##################
    //  #    ENTITIES    #
    //  ##################
    std::vector<TileEntity>          tile_entities = {};
    std::unordered_map<entity_id, Entity> entities = {};

    entity_id last_id = 0;

    void add_entity(const Entity& e);
    void remove_entity(int id);
    
    Entity* get_entity(int id);

    bool collides_with(Entity* entity, Vector2 point);

    //  ###############
    //  #    INPUT    #
    //  ###############
    Vector2i hovered_tile = {};

    //  ##############
    //  #    TOOL    #
    //  ##############
    entity_id current_entity_id = 0;
    
    Vector2i last_pressed_tile = {};
    bool           tool_active = false;
    float            tool_time = 0;

    void draw_sprite(Sprite sprite, Vector2 position, bool flip_x = false, bool flip_y = false);

    //  ##################
    //  #    MAIN API    #
    //  ##################
    void load_resources();
    void init(Vector2i size);
    void update(float delta);
    void render_frame();
};