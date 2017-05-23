#ifndef _item_h_
#define _item_h_

#define EMPTY 0
#define GRASS 1
#define SAND 2
#define STONE_BRICK 3
#define BRICK 4
#define WOOD 5
#define STONE 6
#define DIRT 7
#define PLANK 8
#define SNOW 9
#define GLASS 10
#define COBBLE 11
#define LIGHT_STONE 12
#define DARK_STONE 13
#define CHEST 14
#define LEAVES 15
#define CLOUD 16
#define TALL_GRASS 17
#define YELLOW_FLOWER 18
#define RED_FLOWER 19
#define OAK_SAPLING 20
#define RED_MUSHROOM 21
#define BROWN_MUSHROOM 22
#define DEADBUSH 23
#define SPONGE 24
#define MELON 25
#define END_STONE 26
#define TNT 27
#define EMERALD_BLOCK 28
#define FERN 29
#define SPRUCE_SAPLING 30
#define BIRCH_SAPLING 31
#define WOOL_WHITE 32
#define WOOL_ORANGE 33
#define WOOL_MAGENTA 34
#define WOOL_LIGHT_BLUE 35
#define WOOL_YELLOW 36
#define wOOL_LIME 37
#define WOOL_PINK 38
#define WOOL_GRAY 39
#define WOOL_LIGHT_GRAY 40
#define WOOL_CYAN 41
#define WOOL_PURPLE 42
#define WOOL_BLUE 43
#define WOOL_BROWN 44
#define WOOL_GREEN 45
#define WOOL_RED 46
#define WOOL_BLACK 47
#define DIAMOND_ORE 48
#define REDSTONE_ORE 49
#define BOOKSHELF 50
#define MOSSY_COBBLESTONE 51
#define OBSIDIAN 52
#define WORKBENCH 53
#define FURNACE 54
#define BURNING_FURNACE 55
#define MONSTER_SPAWNER 56
#define SNOW_BLOCK 57
#define ICE 58
#define CLAY 59
#define JUKEBOX 60
#define CACTUS 61
#define MYCELIUM 62
#define NETHERRACK 63
#define GLOWING_STONE 64
#define BEDROCK 65
#define GRAVEL 66
#define IRON_BLOCK 67
#define GOLD_BLOCK 68
#define DIAMOND_BLOCK 69
#define GOLD_ORE 70
#define IRON_ORE 71
#define COAL_ORE 72
#define LAPIS_ORE 73
#define LAPIS_BLOCK 74
#define SANDSTONE 75
#define MOSSY_STONE_BRICK 76
#define CRACKED_STONE_BRICK 77
#define PUMPKIN 78

#define RC(row, col) (row * 32 + col)

extern const int items[];
extern const char* item_names[];
extern const int item_count;
extern const int blocks[256][6];
extern const int plants[256];

bool is_plant(int w);
bool is_obstacle(int w);
bool is_transparent(int w);
bool is_destructable(int w);
int is_illuminated(int w);
float is_hardness(int w);

#endif
