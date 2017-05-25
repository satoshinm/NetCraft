#ifndef _item_h_
#define _item_h_

#define RC(row, col) (row * 32 + col)
#define notexture RC( 0, 0)

extern int hotbar_items[];
extern int hotbar_item_count;
extern const char* block_names[];
extern int block_textures[256][6];

bool is_plant(int w);
bool is_obstacle(int w);
bool is_transparent(int w);
bool is_destructable(int w);
int is_illuminated(int w);
float is_hardness(int w);

void init_blocks(void);
int get_block_data(int id);
bool has_block_data(int id);

#endif
