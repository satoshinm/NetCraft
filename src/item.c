#include <stdbool.h>
#include "item.h"
#include "util.h"

const int items[] = {
    // items the user can build
    GRASS,
    SAND,
    STONE,
    BRICK,
    WOOD,
    CEMENT,
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
    SAPLING,
    RED_MUSHROOM,
    BROWN_MUSHROOM,
    DEADBUSH,
    COLOR_00,
    COLOR_01,
    COLOR_02,
    COLOR_03,
    COLOR_04,
    COLOR_05,
    COLOR_06,
    COLOR_07,
    COLOR_08,
    COLOR_09,
    COLOR_10,
    COLOR_11,
    COLOR_12,
    COLOR_13,
    COLOR_14,
    COLOR_15,
    COLOR_16,
    COLOR_17,
    COLOR_18,
    COLOR_19,
    COLOR_20,
    COLOR_21,
    COLOR_22,
    COLOR_23,
    COLOR_24,
    COLOR_25,
    COLOR_26,
    COLOR_27,
    COLOR_28,
    COLOR_29,
    COLOR_30,
    COLOR_31,
    GLOWING_STONE
};

const char *item_names[] = {
    "EMPTY",
    "GRASS",
    "SAND",
    "STONE",
    "BRICK",
    "WOOD",
    "CEMENT",
    "DIRT",
    "PLANK",
    "SNOW",
    "GLASS",
    "COBBLE",
    "LIGHT_STONE",
    "DARK_STONE",
    "CHEST",
    "LEAVES",
    "CLOUD",
    "TALL_GRASS",
    "YELLOW_FLOWER",
    "RED_FLOWER",
    "SAPLING",
    "RED_MUSHROOM",
    "BROWN_MUSHROOM",
    "DEADBUSH",
    "x24",
    "x25",
    "x26",
    "x27",
    "x28",
    "x29",
    "x30",
    "x31",
    "COLOR_00",
    "COLOR_01",
    "COLOR_02",
    "COLOR_03",
    "COLOR_04",
    "COLOR_05",
    "COLOR_06",
    "COLOR_07",
    "COLOR_08",
    "COLOR_09",
    "COLOR_10",
    "COLOR_11",
    "COLOR_12",
    "COLOR_13",
    "COLOR_14",
    "COLOR_15",
    "COLOR_16",
    "COLOR_17",
    "COLOR_18",
    "COLOR_19",
    "COLOR_20",
    "COLOR_21",
    "COLOR_22",
    "COLOR_23",
    "COLOR_24",
    "COLOR_25",
    "COLOR_26",
    "COLOR_27",
    "COLOR_28",
    "COLOR_29",
    "COLOR_30",
    "COLOR_31",
    "GLOWING_STONE",
    "BEDROCK",
    "GRAVEL",
    "IRON_BLOCK",
    "GOLD_BLOCK",
    "DIAMOND_BLOCK",
};

const int item_count = sizeof(items) / sizeof(int);

const int blocks[256][6] = {
    // w => (front, back, left, right, top, bottom) tiles
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 0 - empty
    {RC(15, 3), RC(15, 3), RC(15, 3), RC(15, 3), RC(15, 0), RC(15, 2)}, // 1 - grass
    {RC(14, 2), RC(14, 2), RC(14, 2), RC(14, 2), RC(14, 2), RC(14, 2)}, // 2 - sand
    {RC(15, 5), RC(15, 5), RC(15, 5), RC(15, 5), RC(15, 5), RC(15, 5)}, // 3 - stone
    {RC(15, 7), RC(15, 7), RC(15, 7), RC(15, 7), RC(15, 7), RC(15, 7)}, // 4 - brick
    {RC(10, 7), RC(10, 7), RC(10, 7), RC(10, 7), RC(14, 5), RC(14, 5)}, // 5 - wood
    {RC(15, 1), RC(15, 1), RC(15, 1), RC(15, 1), RC(15, 1), RC(15, 1)}, // 6 - cement
    {RC(15, 2), RC(15, 2), RC(15, 2), RC(15, 2), RC(15, 2), RC(15, 2)}, // 7 - dirt
    {RC(15, 4), RC(15, 4), RC(15, 4), RC(15, 4), RC(15, 4), RC(15, 4)}, // 8 - plank
    {RC(11, 4), RC(11, 4), RC(11, 4), RC(11, 4), RC( 2, 8), RC(15, 2)}, // 9 - snow
    {RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1)}, // 10 - glass
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
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 24
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 25
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 26
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 27
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 28
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 29
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 30
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 31
    {RC( 5, 2), RC( 5, 2), RC( 5, 2), RC( 5, 2), RC( 5, 2), RC( 5, 2)}, // 32 - yellow
    {RC( 6, 2), RC( 6, 2), RC( 6, 2), RC( 6, 2), RC( 6, 2), RC( 6, 2)}, // 33 - green
    {RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2)}, // 34
    {RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3)}, // 35
    {RC( 6, 1), RC( 6, 1), RC( 6, 1), RC( 6, 1), RC( 6, 1), RC( 6, 1)}, // 36 - dark green
    {RC(11, 5), RC(11, 5), RC(11, 5), RC(11, 5), RC(11, 5), RC(11, 5)}, // 37
    {RC(11, 6), RC(11, 6), RC(11, 6), RC(11, 6), RC(11, 6), RC(11, 6)}, // 38
    {RC(11, 7), RC(11, 7), RC(11, 7), RC(11, 7), RC(11, 7), RC(11, 7)}, // 39
    {RC(11, 8), RC(11, 8), RC(11, 8), RC(11, 8), RC(11, 8), RC(11, 8)}, // 40
    {RC(11, 9), RC(11, 9), RC(11, 9), RC(11, 9), RC(11, 9), RC(11, 9)}, // 41
    {RC(11,10), RC(11,10), RC(11,10), RC(11,10), RC(11,10), RC(11,10)}, // 42
    {RC(11,11), RC(11,11), RC(11,11), RC(11,11), RC(11,11), RC(11,11)}, // 43
    {RC(11,12), RC(11,12), RC(11,12), RC(11,12), RC(11,12), RC(11,12)}, // 44
    {RC(11,13), RC(11,13), RC(11,13), RC(11,13), RC(11,13), RC(11,13)}, // 45
    {RC(11,14), RC(11,14), RC(11,14), RC(11,14), RC(11,14), RC(11,14)}, // 46
    {RC(11,15), RC(11,15), RC(11,15), RC(11,15), RC(11,15), RC(11,15)}, // 47
    {RC(12, 0), RC(12, 0), RC(12, 0), RC(12, 0), RC(12, 0), RC(12, 0)}, // 48
    {RC(12, 1), RC(12, 1), RC(12, 1), RC(12, 1), RC(12, 1), RC(12, 1)}, // 49
    {RC(12, 2), RC(12, 2), RC(12, 2), RC(12, 2), RC(12, 2), RC(12, 2)}, // 50
    {RC(12, 3), RC(12, 3), RC(12, 3), RC(12, 3), RC(12, 3), RC(12, 3)}, // 51
    {RC( 7, 1), RC( 7, 1), RC( 7, 1), RC( 7, 1), RC( 7, 1), RC( 7, 1)}, // 52 - red
    {RC(12, 5), RC(12, 5), RC(12, 5), RC(12, 5), RC(12, 5), RC(12, 5)}, // 53
    {RC(12, 6), RC(12, 6), RC(12, 6), RC(12, 6), RC(12, 6), RC(12, 6)}, // 54
    {RC(12, 7), RC(12, 7), RC(12, 7), RC(12, 7), RC(12, 7), RC(12, 7)}, // 55
    {RC(12, 8), RC(12, 8), RC(12, 8), RC(12, 8), RC(12, 8), RC(12, 8)}, // 56
    {RC(12, 9), RC(12, 9), RC(12, 9), RC(12, 9), RC(12, 9), RC(12, 9)}, // 57
    {RC(12,10), RC(12,10), RC(12,10), RC(12,10), RC(12,10), RC(12,10)}, // 58
    {RC(12,11), RC(12,11), RC(12,11), RC(12,11), RC(12,11), RC(12,11)}, // 59
    {RC(12,12), RC(12,12), RC(12,12), RC(12,12), RC(12,12), RC(12,12)}, // 60
    {RC(12,13), RC(12,13), RC(12,13), RC(12,13), RC(12,13), RC(12,13)}, // 61
    {RC(12,14), RC(12,14), RC(12,14), RC(12,14), RC(12,14), RC(12,14)}, // 62
    {RC(12,15), RC(12,15), RC(12,15), RC(12,15), RC(12,15), RC(12,15)}, // 63
    {RC( 9, 9), RC( 9, 9), RC( 9, 9), RC( 9, 9), RC( 9, 9), RC( 9, 9)}, // 64 - glowing stone
    {RC(14, 1), RC(14, 1), RC(14, 1), RC(14, 1), RC(14, 1), RC(14, 1)}, // 65 - bedrock
    {RC(14, 3), RC(14, 3), RC(14, 3), RC(14, 3), RC(14, 3), RC(14, 3)}, // 66 - gravel
    {RC(14, 6), RC(14, 6), RC(14, 6), RC(14, 6), RC(14, 6), RC(14, 6)}, // 67 - iron block
    {RC(14, 7), RC(14, 7), RC(14, 7), RC(14, 7), RC(14, 7), RC(14, 7)}, // 68 - gold block
    {RC(14, 8), RC(14, 8), RC(14, 8), RC(14, 8), RC(14, 8), RC(14, 8)}, // 69 - diamond block
};

const int plants[256] = {
    // w => tile
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0 - 16
    RC(13, 7), // 17 - tall grass
    RC(15,13), // 18 - yellow flower
    RC(15,12), // 19 - red flower
    RC(15,15), // 20 - sapling
    RC(14,12), // 21 - red mushroom
    RC(14,13), // 22 - brown mushroom
    RC( 9,15), // 23 - deadbush
};

bool is_plant(int w) {
    switch (w) {
        case TALL_GRASS:
        case YELLOW_FLOWER:
        case RED_FLOWER:
        case SAPLING:
        case RED_MUSHROOM:
        case BROWN_MUSHROOM:
        case DEADBUSH:
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
    switch (w) {
        case EMPTY:
        case CLOUD:
            return false;
        default:
            return true;
    }
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
        case STONE:
        case BRICK:
        case CEMENT:
        case COBBLE:
        case LIGHT_STONE:
        case DARK_STONE:
            return 7.0;

        default:
            return 1.0;
    }
}
