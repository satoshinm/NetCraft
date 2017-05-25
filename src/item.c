#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "item.h"
#include "util.h"

int hotbar_items[] = {0};
int hotbar_item_count = 0;

const char *item_names[256] = {0};
int block_count = 0;
int block_textures[256][6] = {{0}};

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

int register_block(char *name, int wfront, int wback, int wleft, int wright, int wtop, int wbottom) {
    int id = block_count++;

    item_names[id] = name;
    block_textures[id][0] = wfront;
    block_textures[id][1] = wback;
    block_textures[id][2] = wleft;
    block_textures[id][3] = wright;
    block_textures[id][4] = wtop;
    block_textures[id][5] = wbottom;

    // TODO: properties

    return id;
}

int block_id_by_name(const char *name) {
    for (int i = 0; i < block_count; ++i) {
        if (strcmp(name, item_names[i]) == 0) return i;
    }
    printf("no such block found: %s\n", name);

    return 0;
}

void init_blocks() {
    //              name              w => (front,     back,     left,        right,    top,       bottom) tiles
    register_block("Empty",               RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0), RC( 0, 0));
    register_block("Grass",               RC(15, 3), RC(15, 3), RC(15, 3), RC(15, 3), RC(15, 0), RC(15, 2));
    register_block("Sand",                RC(14, 2), RC(14, 2), RC(14, 2), RC(14, 2), RC(14, 2), RC(14, 2));
    register_block("Stone Brick",         RC(12, 6), RC(12, 6), RC(12, 6), RC(12, 6), RC(12, 6), RC(12, 6));
    register_block("Brick",               RC(15, 7), RC(15, 7), RC(15, 7), RC(15, 7), RC(15, 7), RC(15, 7));
    register_block("Wood",                RC(14, 4), RC(14, 4), RC(14, 4), RC(14, 4), RC(14, 5), RC(14, 5));
    register_block("Stone",               RC(15, 1), RC(15, 1), RC(15, 1), RC(15, 1), RC(15, 1), RC(15, 1));
    register_block("Dirt",                RC(15, 2), RC(15, 2), RC(15, 2), RC(15, 2), RC(15, 2), RC(15, 2));
    register_block("Plank",               RC(15, 4), RC(15, 4), RC(15, 4), RC(15, 4), RC(15, 4), RC(15, 4));
    register_block("Snow",                RC(11, 4), RC(11, 4), RC(11, 4), RC(11, 4), RC( 2, 8), RC(15, 2));
    register_block("Glass",               RC(12, 1), RC(12, 1), RC(12, 1), RC(12, 1), RC(12, 1), RC(12, 1));
    register_block("Cobble",              RC(14, 0), RC(14, 0), RC(14, 0), RC(14, 0), RC(14, 0), RC(14, 0));
    register_block("Light Stone",         RC( 0,11), RC( 0,11), RC( 0,11), RC( 0,11), RC( 0,11), RC( 0,11));
    register_block("Dark Stone",          RC( 0,12), RC( 0,12), RC( 0,12), RC( 0,12), RC( 0,12), RC( 0,12));
    register_block("Chest",               RC(14,11), RC(14,11), RC(14,11), RC(14,11), RC(14,11), RC(14,11));
    register_block("Leaves",              RC(12, 4), RC(12, 4), RC(12, 4), RC(12, 4), RC(12, 4), RC(12, 4));
    register_block("Cloud",               RC( 0,10), RC( 0,10), RC( 0,10), RC( 0,10), RC( 0,10), RC( 0,10));
    register_block("Tall Grass",          RC(13, 7), notexture, notexture, notexture, notexture, notexture);
    register_block("Yellow Flower",       RC(15,13), notexture, notexture, notexture, notexture, notexture);
    register_block("Red Flower",          RC(15,12), notexture, notexture, notexture, notexture, notexture);
    register_block("Oak Sapling",         RC(15,15), notexture, notexture, notexture, notexture, notexture);
    register_block("Red Mushroom",        RC(14,12), notexture, notexture, notexture, notexture, notexture);
    register_block("Brown Mushroom",      RC(14,13), notexture, notexture, notexture, notexture, notexture);
    register_block("Deadbush",            RC(12, 7), notexture, notexture, notexture, notexture, notexture);
    register_block("Sponge",              RC(12, 0), RC(12, 0), RC(12, 0), RC(12, 0), RC(12, 0), RC(12, 0));
    register_block("Melon",               RC( 7, 8), RC( 7, 8), RC( 7, 8), RC( 7, 8), RC( 7, 9), RC( 7, 9));
    register_block("End Stone",           RC( 5,15), RC( 5,15), RC( 5,15), RC( 5,15), RC( 5,15), RC( 5,15));
    register_block("TNT",                 RC(15, 8), RC(15, 8), RC(15, 8), RC(15, 8), RC(15, 9), RC(15,10));
    register_block("Emerald Block",       RC(14, 9), RC(14, 9), RC(14, 9), RC(14, 9), RC(14, 9), RC(14, 9));
    register_block("Fern",                RC(12, 8), notexture, notexture, notexture, notexture, notexture);
    register_block("Spruce Sapling",      RC(12,15), notexture, notexture, notexture, notexture, notexture);
    register_block("Birch Sapling",       RC(11,15), notexture, notexture, notexture, notexture, notexture);
    register_block("White Wool",          RC(11, 0), RC(11, 0), RC(11, 0), RC(11, 0), RC(11, 0), RC(11, 0));
    register_block("Orange Wool",         RC( 2, 2), RC( 2, 2), RC( 2, 2), RC( 2, 2), RC( 2, 2), RC( 2, 2));
    register_block("Magenta Wool",        RC( 3, 2), RC( 3, 2), RC( 3, 2), RC( 3, 2), RC( 3, 2), RC( 3, 2));
    register_block("Light Blue Wool",     RC( 4, 2), RC( 4, 2), RC( 4, 2), RC( 4, 2), RC( 4, 2), RC( 4, 2));
    register_block("Yellow Wool",         RC( 5, 2), RC( 5, 2), RC( 5, 2), RC( 5, 2), RC( 5, 2), RC( 5, 2));
    register_block("Lime Wool",           RC( 6, 2), RC( 6, 2), RC( 6, 2), RC( 6, 2), RC( 6, 2), RC( 6, 2));
    register_block("Pink Wool",           RC( 7, 2), RC( 7, 2), RC( 7, 2), RC( 7, 2), RC( 7, 2), RC( 7, 2));
    register_block("Gray Wool",           RC( 8, 2), RC( 8, 2), RC( 8, 2), RC( 8, 2), RC( 8, 2), RC( 8, 2));
    register_block("Light Gray Wool",     RC( 1, 1), RC( 1, 1), RC( 1, 1), RC( 1, 1), RC( 1, 1), RC( 1, 1));
    register_block("Cyan Wool",           RC( 2, 1), RC( 2, 1), RC( 2, 1), RC( 2, 1), RC( 2, 1), RC( 2, 1));
    register_block("Purple Wool",         RC( 3, 1), RC( 3, 1), RC( 3, 1), RC( 3, 1), RC( 3, 1), RC( 3, 1));
    register_block("Blue Wool",           RC( 4, 1), RC( 4, 1), RC( 4, 1), RC( 4, 1), RC( 4, 1), RC( 4, 1));
    register_block("Brown Wool",          RC( 5, 1), RC( 5, 1), RC( 5, 1), RC( 5, 1), RC( 5, 1), RC( 5, 1));
    register_block("Green Wool",          RC( 6, 1), RC( 6, 1), RC( 6, 1), RC( 6, 1), RC( 6, 1), RC( 6, 1));
    register_block("Red Wool",            RC( 7, 1), RC( 7, 1), RC( 7, 1), RC( 7, 1), RC( 7, 1), RC( 7, 1));
    register_block("Black Wool",          RC( 8, 1), RC( 8, 1), RC( 8, 1), RC( 8, 1), RC( 8, 1), RC( 8, 1));
    register_block("Diamond Ore",         RC(12, 2), RC(12, 2), RC(12, 2), RC(12, 2), RC(12, 2), RC(12, 2));
    register_block("Redstone Ore",        RC(12, 3), RC(12, 3), RC(12, 3), RC(12, 3), RC(12, 3), RC(12, 3));
    register_block("Bookshelf",           RC(13, 3), RC(13, 3), RC(13, 3), RC(13, 3), RC(15, 4), RC(15, 4));
    register_block("Mossy Cobblestone",   RC(13, 4), RC(13, 4), RC(13, 4), RC(13, 4), RC(13, 4), RC(13, 4));
    register_block("Obsidian",            RC(13, 5), RC(13, 5), RC(13, 5), RC(13, 5), RC(13, 5), RC(13, 5));
    register_block("Workbench",           RC(12,12), RC(12,11), RC(12,11), RC(12,11), RC(13,11), RC(12,11));
    register_block("Furnace",             RC(13,12), RC(13,13), RC(13,13), RC(13,13), RC(12,14), RC(13,13));
    register_block("Burning Furnace",     RC(12,13), RC(13,13), RC(13,13), RC(13,13), RC(12,14), RC(13,13));
    register_block("Monster Spawner",     RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1), RC(11, 1));
    register_block("Snow Block",          RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2), RC(11, 2));
    register_block("Ice",                 RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3), RC(11, 3));
    register_block("Clay",                RC(11, 8), RC(11, 8), RC(11, 8), RC(11, 8), RC(11, 8), RC(11, 8));
    register_block("Jukebox",             RC(11,10), RC(11,10), RC(11,10), RC(11,10), RC(11,11), RC(11,10));
    register_block("Cactus",              RC(11, 6), RC(11, 6), RC(11, 6), RC(11, 6), RC(11, 5), RC(11, 7));
    register_block("Mycelium",            RC(11,13), RC(11,13), RC(11,13), RC(11,13), RC(11,14), RC(11,13));
    register_block("Netherrack",          RC( 9, 7), RC( 9, 7), RC( 9, 7), RC( 9, 7), RC( 9, 7), RC( 9, 7));
    register_block("Glowstone",           RC( 9, 9), RC( 9, 9), RC( 9, 9), RC( 9, 9), RC( 9, 9), RC( 9, 9));
    register_block("Bedrock",             RC(14, 1), RC(14, 1), RC(14, 1), RC(14, 1), RC(14, 1), RC(14, 1));
    register_block("Gravel",              RC(14, 3), RC(14, 3), RC(14, 3), RC(14, 3), RC(14, 3), RC(14, 3));
    register_block("Iron Block",          RC(14, 6), RC(14, 6), RC(14, 6), RC(14, 6), RC(14, 6), RC(14, 6));
    register_block("Gold Block",          RC(14, 7), RC(14, 7), RC(14, 7), RC(14, 7), RC(14, 7), RC(14, 7));
    register_block("Diamond Block",       RC(14, 8), RC(14, 8), RC(14, 8), RC(14, 8), RC(14, 8), RC(14, 8));
    register_block("Gold Ore",            RC(13, 0), RC(13, 0), RC(13, 0), RC(13, 0), RC(13, 0), RC(13, 0));
    register_block("Iron Ore",            RC(13, 1), RC(13, 1), RC(13, 1), RC(13, 1), RC(13, 1), RC(13, 1));
    register_block("Coal Ore",            RC(13, 2), RC(13, 2), RC(13, 2), RC(13, 2), RC(13, 2), RC(13, 2));
    register_block("Lapis Lazuli Ore",    RC( 5, 0), RC( 5, 0), RC( 5, 0), RC( 5, 0), RC( 5, 0), RC( 5, 0));
    register_block("Lapis Lazuli Block",  RC( 6, 0), RC( 6, 0), RC( 6, 0), RC( 6, 0), RC( 6, 0), RC( 6, 0));
    register_block("Sandstone",           RC( 3, 0), RC( 3, 0), RC( 3, 0), RC( 3, 0), RC( 4, 0), RC( 2, 0));
    register_block("Mossy Stone Brick",   RC( 9, 4), RC( 9, 4), RC( 9, 4), RC( 9, 4), RC( 9, 4), RC( 9, 4));
    register_block("Cracked Stone Brick", RC( 9, 5), RC( 9, 5), RC( 9, 5), RC( 9, 5), RC( 9, 5), RC( 9, 5));
    register_block("Pumpkin",             RC( 8, 6), RC( 8, 6), RC( 8, 6), RC( 8, 6), RC( 9, 6), RC( 9, 6));
    register_block("Jack-o'-Lantern",     RC( 8, 8), RC( 8, 8), RC( 8, 8), RC( 8, 8), RC( 9, 6), RC( 9, 6));
    register_block("Huge Brown Mushroom", RC( 8,14), RC( 8,14), RC( 8,14), RC( 8,14), RC( 8,14), RC( 8,14));
    register_block("Huge Red Mushroom",   RC( 8,13), RC( 8,13), RC( 8,13), RC( 8,13), RC( 8,13), RC( 8,13));
    register_block("Command Block",       RC( 4, 8), RC( 4, 8), RC( 4, 8), RC( 4, 8), RC( 4, 8), RC( 4, 8));
    register_block("Emerald Ore",         RC( 5,11), RC( 5,11), RC( 5,11), RC( 5,11), RC( 5,11), RC( 5,11));
    register_block("Soul Sand",           RC( 9, 8), RC( 9, 8), RC( 9, 8), RC( 9, 8), RC( 9, 8), RC( 9, 8));
    register_block("Nether Brick",        RC( 1, 0), RC( 1, 0), RC( 1, 0), RC( 1, 0), RC( 1, 0), RC( 1, 0));
    register_block("Wet Farmland",        RC(15, 2), RC(15, 2), RC(15, 2), RC(15, 2), RC(10, 6), RC(15, 2));
    register_block("Dry Farmland",        RC(15, 2), RC(15, 2), RC(15, 2), RC(15, 2), RC(10, 7), RC(15, 2));
    register_block("Lamp Off",            RC( 2, 3), RC( 2, 3), RC( 2, 3), RC( 2, 3), RC( 2, 3), RC( 2, 3));
    register_block("Lamp On",             RC( 2, 4), RC( 2, 4), RC( 2, 4), RC( 2, 4), RC( 2, 4), RC( 2, 4));

    // hotbar_items the user can build
    hotbar_items[hotbar_item_count++] = block_id_by_name("Grass");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Sand");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Stone Brick");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Brick");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Wood");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Stone");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Dirt");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Plank");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Snow");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Glass");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Cobble");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Light Stone");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Dark Stone");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Chest");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Leaves");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Tall Grass");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Yellow Flower");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Red Flower");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Oak Sapling");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Red Mushroom");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Brown Mushroom");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Deadbush");
    hotbar_items[hotbar_item_count++] = block_id_by_name("White Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Orange Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Magenta Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Light Blue Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Yellow Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Lime Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Pink Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Gray Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Light Gray Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Cyan Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Purple Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Blue Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Brown Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Green Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Red Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Black Wool");
    hotbar_items[hotbar_item_count++] = block_id_by_name("Glowstone");
}
