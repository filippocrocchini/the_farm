#pragma once

#include "my_math.h"

#include <vector>

constexpr int UNIT_TO_PIXELS = 32;

constexpr int TILE_SIZE_UNITS  = 1;
constexpr int TILE_SIZE_PIXELS = TILE_SIZE_UNITS * UNIT_TO_PIXELS;

struct GameState;

struct Tool
{
    const char* name = "";
    float max_time = 0;
    virtual void action(GameState* game_state) = 0;

    constexpr explicit Tool(const char* name, float max_time) : name{ name }, max_time { max_time } {}

    constexpr Tool() = default;
    constexpr Tool(const Tool&) = default;
    constexpr Tool(Tool&&) = default;
    constexpr Tool& operator=(const Tool&) = default;
    constexpr Tool& operator=(Tool&&) = default;

    virtual ~Tool() = default;
};

struct Harvest : Tool {
    constexpr Harvest(const char* name, float max_time) : Tool{ name, max_time } {}
    void action(GameState* game_state) override;
};

struct Plant : Tool {
    constexpr Plant(const char* name, float max_time) : Tool{ name, max_time } {}
    void action(GameState* game_state) override;
};

struct Plow : Tool {
    constexpr Plow(const char* name, float max_time) : Tool{ name, max_time } {}
    void action(GameState* game_state) override;
};

//  ###############
//  #    TOOLS    #
//  ###############
static auto HARVEST_TOOL = Harvest{ "Harvest", 0.2f };
static auto PLANT_TOOL   = Plant{ "Plant", 0.2f };
static auto PLOW_TOOL    = Plow{ "Plow", 0.2f };

struct Tile
{
    bool plowed = false;
};

struct TileEntity
{
    Vector2i tile_position = {};
    bool       fully_grown = false;
    bool              dead = false;
    float       spawn_time = 0;
};

struct ToolEntity
{
    Tool* tool = nullptr;
    Vector2 position = {};

    constexpr ToolEntity(Tool* tool, Vector2 position) : tool{ tool }, position{position} {}
    constexpr ToolEntity() = default;
};

struct GameState
{
    Camera2D       camera = {};
    float time_from_start = 0;
    Vector2i  render_size = {};

    //  ###############
    //  #    TILES    #
    //  ###############
    Vector2i     level_size = {};
    std::vector<Tile> tiles = {};
 
    //  ##################
    //  #    ENTITIES    #
    //  ##################
    std::vector<TileEntity> tile_entities = {};
    std::vector<ToolEntity> entities = {};

    //  ###############
    //  #    INPUT    #
    //  ###############
    Vector2i hovered_tile = {};

    //  ##############
    //  #    TOOL    #
    //  ##############
    ToolEntity* current_tool_entity = nullptr; // TODO: Remove this, pointer is bad
    
    Tool*         current_tool = nullptr;
    Vector2i last_pressed_tile = {};
    bool           tool_active = false;
    float            tool_time = 0;

    void init(Vector2i size);

    void update(float delta);
    void render_frame();
};