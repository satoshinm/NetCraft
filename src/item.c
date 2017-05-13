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
    PURPLE_FLOWER,
    SUN_FLOWER,
    WHITE_FLOWER,
    BLUE_FLOWER,
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
    "PURPLE_FLOWER",
    "SUN_FLOWER",
    "WHITE_FLOWER",
    "BLUE_FLOWER",
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
};

const int item_count = sizeof(items) / sizeof(int);

#define RC(row, col) (row * 16 + col)
const int blocks[256][6] = {
    // w => (front, back, left, right, top, bottom) tiles
    {RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0)}, // 0 - empty
    {RC( 1, 0), RC( 1, 0), RC( 1, 0), RC( 1, 0), RC( 2, 0), RC( 0, 0)}, // 1 - grass
    {RC( 0, 1), RC( 0, 1), RC( 0, 1), RC( 0, 1), RC( 0, 1), RC( 0, 1)}, // 2 - sand
    {RC( 0, 2), RC( 0, 2), RC( 0, 2), RC( 0, 2), RC( 0, 2), RC( 0, 2)}, // 3 - stone
    {RC( 0, 3), RC( 0, 3), RC( 0, 3), RC( 0, 3), RC( 0, 3), RC( 0, 3)}, // 4 - brick
    {RC( 1, 4), RC( 1, 4), RC( 1, 4), RC( 1, 4), RC( 2, 4), RC( 0, 4)}, // 5 - wood
    {RC( 0, 5), RC( 0, 5), RC( 0, 5), RC( 0, 5), RC( 0, 5), RC( 0, 5)}, // 6 - cement
    {RC( 0, 6), RC( 0, 6), RC( 0, 6), RC( 0, 6), RC( 0, 6), RC( 0, 6)}, // 7 - dirt
    {RC( 0, 7), RC( 0, 7), RC( 0, 7), RC( 0, 7), RC( 0, 7), RC( 0, 7)}, // 8 - plank
    {RC( 1, 8), RC( 1, 8), RC( 1, 8), RC( 1, 8), RC( 2, 8), RC( 0, 8)}, // 9 - snow
    {RC( 0, 9), RC( 0, 9), RC( 0, 9), RC( 0, 9), RC( 0, 9), RC( 0, 9)}, // 10 - glass
    {RC( 0,10), RC( 0,10), RC( 0,10), RC( 0,10), RC( 0,10), RC( 0,10)}, // 11 - cobble
    {RC( 0,11), RC( 0,11), RC( 0,11), RC( 0,11), RC( 0,11), RC( 0,11)}, // 12 - light stone
    {RC( 0,12), RC( 0,12), RC( 0,12), RC( 0,12), RC( 0,12), RC( 0,12)}, // 13 - dark stone
    {RC( 0,13), RC( 0,13), RC( 0,13), RC( 0,13), RC( 0,13), RC( 0,13)}, // 14 - chest
    {RC( 0,14), RC( 0,14), RC( 0,14), RC( 0,14), RC( 0,14), RC( 0,14)}, // 15 - leaves
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
    {RC(11, 0), RC(11, 0), RC(11, 0), RC(11, 0), RC(11, 0), RC(11, 0)}, // 32
    {RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1)}, // 33
    {RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2)}, // 34
    {RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3)}, // 35
    {RC(11, 4), RC(11, 4), RC(11, 4), RC(11, 4), RC(11, 4), RC(11, 4)}, // 36
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
    {RC(12, 4), RC(12, 4), RC(12, 4), RC(12, 4), RC(12, 4), RC(12, 4)}, // 52
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
    {RC( 1, 1), RC( 1, 1), RC( 1, 1), RC( 1, 1), RC( 1, 1), RC( 1, 1)}, // 64
};

const int plants[256] = {
    // w => tile
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0 - 16
    48, // 17 - tall grass
    49, // 18 - yellow flower
    50, // 19 - red flower
    51, // 20 - purple flower
    52, // 21 - sun flower
    53, // 22 - white flower
    54, // 23 - blue flower
};

bool is_plant(int w) {
    switch (w) {
        case TALL_GRASS:
        case YELLOW_FLOWER:
        case RED_FLOWER:
        case PURPLE_FLOWER:
        case SUN_FLOWER:
        case WHITE_FLOWER:
        case BLUE_FLOWER:
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
