#include <math.h>
#include <stdbool.h>
#include "item.h"
#include "util.h"

const int items[] = {
    // items the user can build
    GRASS,
    SAND,
    STONE_BRICK,
    BRICK,
    WOOD,
    STONE,
    DIRT,
    PLANK,
    SNOW,
    GLASS,
    COBBLE,
    LIGHT_STONE,
    DARK_STONE,
    CHEST,
    LEAVES,
    TALL_GRASS,
    YELLOW_FLOWER,
    RED_FLOWER,
    OAK_SAPLING,
    RED_MUSHROOM,
    BROWN_MUSHROOM,
    DEADBUSH,
    WOOL_WHITE,
    WOOL_ORANGE,
    WOOL_MAGENTA,
    WOOL_LIGHT_BLUE,
    WOOL_YELLOW,
    wOOL_LIME,
    WOOL_PINK,
    WOOL_GRAY,
    WOOL_LIGHT_GRAY,
    WOOL_CYAN,
    WOOL_PURPLE,
    WOOL_BLUE,
    WOOL_BROWN,
    WOOL_GREEN,
    WOOL_RED,
    WOOL_BLACK,
    GLOWING_STONE
};

const char *item_names[] = {
    "Empty",
    "Grass",
    "Sand",
    "Stone Brick",
    "Brick",
    "Wood",
    "Stone",
    "Dirt",
    "Plank",
    "Snow",
    "Glass",
    "Cobble",
    "Light Stone",
    "Dark Stone",
    "Chest",
    "Leaves",
    "Cloud",
    "Tall Grass",
    "Yellow Flower",
    "Red Flower",
    "Oak Sapling",
    "Red Mushroom",
    "Brown Mushroom",
    "Deadbush",
    "Sponge",
    "Melon",
    "End Stone",
    "TNT",
    "Emerald Block",
    "Fern",
    "Spruce Sapling",
    "Birch Sapling",
    "White Wool",
    "Orange Wool",
    "Magenta Wool",
    "Light Blue Wool",
    "Yellow Wool",
    "Lime Wool",
    "Pink Wool",
    "Gray Wool",
    "Light Gray Wool",
    "Cyan Wool",
    "Purple Wool",
    "Blue Wool",
    "Brown Wool",
    "Green Wool",
    "Red Wool",
    "Black Wool",
    "Diamond Ore",
    "Redstone Ore",
    "Bookshelf",
    "Mossy Cobblestone",
    "Obsidian",
    "Workbench",
    "Furnace",
    "Burning Furnace",
    "Monster Spawner",
    "Snow Block",
    "Ice",
    "Clay",
    "Jukebox",
    "Cactus",
    "Mycelium",
    "Netherrack",
    "Glowstone",
    "Bedrock",
    "Gravel",
    "Iron Block",
    "Gold Block",
    "Diamond Block",
    "Gold Ore",
    "Iron Ore",
    "Coal Ore",
    "Lapis Lazuli Ore",
    "Lapis Lazuli Block",
    "Sandstone",
    "Mossy Stone Brick",
    "Cracked Stone Brick",
    "Pumpkin",
};

const int item_count = sizeof(items) / sizeof(int);

const int blocks[256][6] = {
    // w => (front, back, left, right, top, bottom) tiles
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 0 - empty
    {RC(15, 3), RC(15, 3), RC(15, 3), RC(15, 3), RC(15, 0), RC(15, 2)}, // 1 - grass
    {RC(14, 2), RC(14, 2), RC(14, 2), RC(14, 2), RC(14, 2), RC(14, 2)}, // 2 - sand
    {RC(12, 6), RC(12, 6), RC(12, 6), RC(12, 6), RC(12, 6), RC(12, 6)}, // 3 - stone brick
    {RC(15, 7), RC(15, 7), RC(15, 7), RC(15, 7), RC(15, 7), RC(15, 7)}, // 4 - brick
    {RC(14, 4), RC(14, 4), RC(14, 4), RC(14, 4), RC(14, 5), RC(14, 5)}, // 5 - wood
    {RC(15, 1), RC(15, 1), RC(15, 1), RC(15, 1), RC(15, 1), RC(15, 1)}, // 6 - stone
    {RC(15, 2), RC(15, 2), RC(15, 2), RC(15, 2), RC(15, 2), RC(15, 2)}, // 7 - dirt
    {RC(15, 4), RC(15, 4), RC(15, 4), RC(15, 4), RC(15, 4), RC(15, 4)}, // 8 - plank
    {RC(11, 4), RC(11, 4), RC(11, 4), RC(11, 4), RC( 2, 8), RC(15, 2)}, // 9 - snow
    {RC(12, 1), RC(12, 1), RC(12, 1), RC(12, 1), RC(12, 1), RC(12, 1)}, // 10 - glass
    {RC(14, 0), RC(14, 0), RC(14, 0), RC(14, 0), RC(14, 0), RC(14, 0)}, // 11 - cobble
    {RC( 0,11), RC( 0,11), RC( 0,11), RC( 0,11), RC( 0,11), RC( 0,11)}, // 12 - light stone
    {RC( 0,12), RC( 0,12), RC( 0,12), RC( 0,12), RC( 0,12), RC( 0,12)}, // 13 - dark stone
    {RC(14,11), RC(14,11), RC(14,11), RC(14,11), RC(14,11), RC(14,11)}, // 14 - chest
    {RC(12, 4), RC(12, 4), RC(12, 4), RC(12, 4), RC(12, 4), RC(12, 4)}, // 15 - leaves
    {RC( 0,15), RC( 0,15), RC( 0,15), RC( 0,15), RC( 0,15), RC( 0,15)}, // 16 - cloud
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 17
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 18
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 19
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 20
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 21
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 22
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 23
    {RC(12, 0), RC(12, 0), RC(12, 0), RC(12, 0), RC(12, 0), RC(12, 0)}, // 24 - sponge
    {RC( 7, 8), RC( 7, 8), RC( 7, 8), RC( 7, 8), RC( 7, 9), RC( 7, 9)}, // 25 - melon
    {RC( 5,15), RC( 5,15), RC( 5,15), RC( 5,15), RC( 5,15), RC( 5,15)}, // 26 - end stone
    {RC(15, 8), RC(15, 8), RC(15, 8), RC(15, 8), RC(15, 9), RC(15,10)}, // 27 - tnt
    {RC(14, 9), RC(14, 9), RC(14, 9), RC(14, 9), RC(14, 9), RC(14, 9)}, // 28 - emerald block
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 29
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 30
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 31
    {RC(11, 0), RC(11, 0), RC(11, 0), RC(11, 0), RC(11, 0), RC(11, 0)}, // 32 - white
    {RC( 2, 2), RC( 2, 2), RC( 2, 2), RC( 2, 2), RC( 2, 2), RC( 2, 2)}, // 33 - orange
    {RC( 3, 2), RC( 3, 2), RC( 3, 2), RC( 3, 2), RC( 3, 2), RC( 3, 2)}, // 34 - magenta
    {RC( 4, 2), RC( 4, 2), RC( 4, 2), RC( 4, 2), RC( 4, 2), RC( 4, 2)}, // 35 - light blue
    {RC( 5, 2), RC( 5, 2), RC( 5, 2), RC( 5, 2), RC( 5, 2), RC( 5, 2)}, // 36 - yellow
    {RC( 6, 2), RC( 6, 2), RC( 6, 2), RC( 6, 2), RC( 6, 2), RC( 6, 2)}, // 37 - lime
    {RC( 7, 2), RC( 7, 2), RC( 7, 2), RC( 7, 2), RC( 7, 2), RC( 7, 2)}, // 38 - pink
    {RC( 8, 2), RC( 8, 2), RC( 8, 2), RC( 8, 2), RC( 8, 2), RC( 8, 2)}, // 39 - gray
    {RC( 1, 1), RC( 1, 1), RC( 1, 1), RC( 1, 1), RC( 1, 1), RC( 1, 1)}, // 40 - light gray (silver)
    {RC( 2, 1), RC( 2, 1), RC( 2, 1), RC( 2, 1), RC( 2, 1), RC( 2, 1)}, // 41 - cyan
    {RC( 3, 1), RC( 3, 1), RC( 3, 1), RC( 3, 1), RC( 3, 1), RC( 3, 1)}, // 42 - purple
    {RC( 4, 1), RC( 4, 1), RC( 4, 1), RC( 4, 1), RC( 4, 1), RC( 4, 1)}, // 43 - blue
    {RC( 5, 1), RC( 5, 1), RC( 5, 1), RC( 5, 1), RC( 5, 1), RC( 5, 1)}, // 44 - brown
    {RC( 6, 1), RC( 6, 1), RC( 6, 1), RC( 6, 1), RC( 6, 1), RC( 6, 1)}, // 45 - green
    {RC( 7, 1), RC( 7, 1), RC( 7, 1), RC( 7, 1), RC( 7, 1), RC( 7, 1)}, // 46 - red
    {RC( 8, 1), RC( 8, 1), RC( 8, 1), RC( 8, 1), RC( 8, 1), RC( 8, 1)}, // 47 - black
    {RC(12, 2), RC(12, 2), RC(12, 2), RC(12, 2), RC(12, 2), RC(12, 2)}, // 48 - diamond ore
    {RC(12, 3), RC(12, 3), RC(12, 3), RC(12, 3), RC(12, 3), RC(12, 3)}, // 49 - redstone ore
    {RC(13, 3), RC(13, 3), RC(13, 3), RC(13, 3), RC(15, 4), RC(15, 4)}, // 50 - bookshelf
    {RC(13, 4), RC(13, 4), RC(13, 4), RC(13, 4), RC(13, 4), RC(13, 4)}, // 51 - mossy cobblestone
    {RC(13, 5), RC(13, 5), RC(13, 5), RC(13, 5), RC(13, 5), RC(13, 5)}, // 52 - obsidian
    {RC(12,12), RC(12,11), RC(12,11), RC(12,11), RC(13,11), RC(12,11)}, // 53 - workbench
    {RC(13,12), RC(13,13), RC(13,13), RC(13,13), RC(12,14), RC(13,13)}, // 54 - furnace
    {RC(12,13), RC(13,13), RC(13,13), RC(13,13), RC(12,14), RC(13,13)}, // 55 - burning furnace
    {RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1)}, // 56 - monster spawner
    {RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2)}, // 57 - snow block
    {RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3)}, // 58 - ice
    {RC(11, 8), RC(11, 8), RC(11, 8), RC(11, 8), RC(11, 8), RC(11, 8)}, // 59 - clay
    {RC(11,10), RC(11,10), RC(11,10), RC(11,10), RC(11,11), RC(11,10)}, // 60 - jukebox
    {RC(11, 6), RC(11, 6), RC(11, 6), RC(11, 6), RC(11, 5), RC(11, 7)}, // 61 - cactus TODO: smaller model
    {RC(11,13), RC(11,13), RC(11,13), RC(11,13), RC(11,14), RC(11,13)}, // 62 - mycelium
    {RC( 9, 7), RC( 9, 7), RC( 9, 7), RC( 9, 7), RC( 9, 7), RC( 9, 7)}, // 63 - netherrack
    {RC( 9, 9), RC( 9, 9), RC( 9, 9), RC( 9, 9), RC( 9, 9), RC( 9, 9)}, // 64 - glowstone
    {RC(14, 1), RC(14, 1), RC(14, 1), RC(14, 1), RC(14, 1), RC(14, 1)}, // 65 - bedrock
    {RC(14, 3), RC(14, 3), RC(14, 3), RC(14, 3), RC(14, 3), RC(14, 3)}, // 66 - gravel
    {RC(14, 6), RC(14, 6), RC(14, 6), RC(14, 6), RC(14, 6), RC(14, 6)}, // 67 - iron block
    {RC(14, 7), RC(14, 7), RC(14, 7), RC(14, 7), RC(14, 7), RC(14, 7)}, // 68 - gold block
    {RC(14, 8), RC(14, 8), RC(14, 8), RC(14, 8), RC(14, 8), RC(14, 8)}, // 69 - diamond block
    {RC(13, 0), RC(13, 0), RC(13, 0), RC(13, 0), RC(13, 0), RC(13, 0)}, // 70 - gold ore
    {RC(13, 1), RC(13, 1), RC(13, 1), RC(13, 1), RC(13, 1), RC(13, 1)}, // 71 - iron ore
    {RC(13, 2), RC(13, 2), RC(13, 2), RC(13, 2), RC(13, 2), RC(13, 2)}, // 72 - coal ore
    {RC( 5, 0), RC( 5, 0), RC( 5, 0), RC( 5, 0), RC( 5, 0), RC( 5, 0)}, // 73 - lapis ore
    {RC( 6, 0), RC( 6, 0), RC( 6, 0), RC( 6, 0), RC( 6, 0), RC( 6, 0)}, // 74 - lapis block
    {RC( 3, 0), RC( 3, 0), RC( 3, 0), RC( 3, 0), RC( 4, 0), RC( 2, 0)}, // 75 - sandstone
    {RC( 9, 4), RC( 9, 4), RC( 9, 4), RC( 9, 4), RC( 9, 4), RC( 9, 4)}, // 76 - mossy stone brick
    {RC( 9, 5), RC( 9, 5), RC( 9, 5), RC( 9, 5), RC( 9, 5), RC( 9, 5)}, // 77 - cracked stone brick
    {RC( 8, 6), RC( 8, 6), RC( 8, 6), RC( 8, 6), RC( 9, 6), RC( 9, 6)}, // 78 - pumpkin (TODO: face)
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)},
};

const int plants[256] = {
    // w => tile
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0 - 16
    RC(13, 7), // 17 - tall grass
    RC(15,13), // 18 - yellow flower
    RC(15,12), // 19 - red flower
    RC(15,15), // 20 - oak sapling
    RC(14,12), // 21 - red mushroom
    RC(14,13), // 22 - brown mushroom
    RC(12, 7), // 23 - deadbush
    0, 0, 0, 0, 0, 0,
    RC(12, 8), // 29 - fern
    RC(12,15), // 30 - spruce sapling
    RC(11,15), // 31 - birch sapling
};

bool is_plant(int w) {
    switch (w) {
        case TALL_GRASS:
        case YELLOW_FLOWER:
        case RED_FLOWER:
        case OAK_SAPLING:
        case RED_MUSHROOM:
        case BROWN_MUSHROOM:
        case DEADBUSH:
        case FERN:
        case SPRUCE_SAPLING:
        case BIRCH_SAPLING:
            return true;
        default:
            return false;
    }
}

bool is_obstacle(int w) {
    w = ABS(w);
    if (is_plant(w)) {
        return false;
    }
    switch (w) {
        case EMPTY:
        case CLOUD:
            return false;
        default:
            return true;
    }
}

bool is_transparent(int w) {
    if (w == EMPTY) {
        return true;
    }
    w = ABS(w);
    if (is_plant(w)) {
        return true;
    }
    switch (w) {
        case EMPTY:
        case GLASS:
        case LEAVES:
            return true;
        default:
            return false;
    }
}

bool is_destructable(int w) {
    return isfinite(is_hardness(w));
}

int is_illuminated(int w) {
    switch (w) {
        case GLOWING_STONE:
            return 15;
        default:
            return 0;
    }
}

// Get block hardness in seconds to mine
float is_hardness(int w) {
    if (is_plant(w)) return 0;

    switch (w) {
        case LEAVES:
            return 0.2;
        case GRASS:
        case DIRT:
        case SAND:
            return 0.5;
        case WOOD:
        case PLANK:
        case CHEST:
            return 3.0;
        case GLASS:
            return 0.4;
        case STONE_BRICK:
        case BRICK:
        case STONE:
        case COBBLE:
        case LIGHT_STONE:
        case DARK_STONE:
            return 7.0;
        case EMPTY:
        case CLOUD:
        case BEDROCK:
            return INFINITY;

        default:
            return 1.0;
    }
}
